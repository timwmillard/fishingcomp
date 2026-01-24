package main

import (
	"flag"
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/getkin/kin-openapi/openapi3"
)

type NamingStyle string

const (
	SnakeCase  NamingStyle = "snake"
	CamelCase  NamingStyle = "camel"
	PascalCase NamingStyle = "pascal"
)

type Generator struct {
	doc         *openapi3.T
	out         strings.Builder
	structStyle NamingStyle
	fieldStyle  NamingStyle
	funcStyle   NamingStyle
}

func main() {
	specPath := flag.String("spec", "openapi.yaml", "Path to OpenAPI spec file")
	output := flag.String("output", "api.h", "Output header file")
	structStyle := flag.String("struct-style", "pascal", "Naming style for structs: snake, camel, pascal")
	fieldStyle := flag.String("field-style", "snake", "Naming style for fields: snake, camel, pascal")
	funcStyle := flag.String("func-style", "snake", "Naming style for functions: snake, camel, pascal")
	flag.Parse()

	loader := openapi3.NewLoader()
	doc, err := loader.LoadFromFile(*specPath)
	if err != nil {
		fmt.Fprintf(os.Stderr, "Error loading spec: %v\n", err)
		os.Exit(1)
	}

	if err := doc.Validate(loader.Context); err != nil {
		fmt.Fprintf(os.Stderr, "Invalid spec: %v\n", err)
		os.Exit(1)
	}

	gen := &Generator{
		doc:         doc,
		structStyle: NamingStyle(*structStyle),
		fieldStyle:  NamingStyle(*fieldStyle),
		funcStyle:   NamingStyle(*funcStyle),
	}

	gen.generate(*output)

	if err := os.WriteFile(*output, []byte(gen.out.String()), 0644); err != nil {
		fmt.Fprintf(os.Stderr, "Error writing output: %v\n", err)
		os.Exit(1)
	}

	fmt.Printf("Generated %s\n", *output)
}

func (g *Generator) generate(outputFile string) {
	// Header guard
	guardName := strings.ToUpper(strings.ReplaceAll(outputFile, ".", "_"))
	g.out.WriteString(fmt.Sprintf("#ifndef %s\n", guardName))
	g.out.WriteString(fmt.Sprintf("#define %s\n\n", guardName))
	g.out.WriteString("// Generated from OpenAPI spec - do not edit\n\n")
	g.out.WriteString("#include <stdint.h>\n")
	g.out.WriteString("#include <stdbool.h>\n")
	g.out.WriteString("#include <stddef.h>\n\n")

	// Forward declarations for Req/Res
	g.out.WriteString("// Forward declarations\n")
	g.out.WriteString("typedef struct Req Req;\n")
	g.out.WriteString("typedef struct Res Res;\n\n")

	// Forward declare all structs first (for circular references)
	g.out.WriteString("// Struct forward declarations\n")
	g.generateForwardDeclarations()

	// Generate structs from schemas
	g.out.WriteString("// ============ Structs ============\n\n")
	g.generateStructs()

	// Generate handler signatures from paths
	g.out.WriteString("// ============ Handlers ============\n\n")
	g.generateHandlers()

	// Close header guard
	g.out.WriteString(fmt.Sprintf("#endif // %s\n", guardName))
}

func (g *Generator) generateForwardDeclarations() {
	names := g.sortedSchemaNames()
	for _, name := range names {
		schema := g.doc.Components.Schemas[name]
		if schema.Value != nil && schema.Value.Type.Is("object") {
			structName := g.applyStyle(name, g.structStyle)
			g.out.WriteString(fmt.Sprintf("typedef struct %s %s;\n", structName, structName))
		}
	}
	g.out.WriteString("\n")
}

func (g *Generator) generateStructs() {
	names := g.sortedSchemaNames()
	for _, name := range names {
		schema := g.doc.Components.Schemas[name]
		if schema.Value == nil {
			continue
		}
		if schema.Value.Type.Is("object") {
			g.generateStruct(name, schema.Value)
		}
	}
}

func (g *Generator) generateStruct(name string, schema *openapi3.Schema) {
	structName := g.applyStyle(name, g.structStyle)

	// Schema description as comment
	if schema.Description != "" {
		g.out.WriteString(fmt.Sprintf("// %s\n", wrapComment(schema.Description)))
	}

	g.out.WriteString(fmt.Sprintf("struct %s {\n", structName))

	// Sort properties for deterministic output
	propNames := g.sortedPropertyNames(schema.Properties)

	for _, propName := range propNames {
		prop := schema.Properties[propName]
		g.generateField(propName, prop)
	}

	g.out.WriteString(fmt.Sprintf("};\n\n"))
}

func (g *Generator) generateField(propName string, propRef *openapi3.SchemaRef) {
	fieldName := g.applyStyle(propName, g.fieldStyle)

	// Field description as comment
	if propRef.Value != nil && propRef.Value.Description != "" {
		g.out.WriteString(fmt.Sprintf("    // %s\n", wrapComment(propRef.Value.Description)))
	}

	// Check if it's a $ref to another schema
	if propRef.Ref != "" {
		refType := g.resolveRefType(propRef.Ref)
		g.out.WriteString(fmt.Sprintf("    %s *%s;\n", refType, fieldName))
		return
	}

	if propRef.Value == nil {
		g.out.WriteString(fmt.Sprintf("    void *%s;\n", fieldName))
		return
	}

	schema := propRef.Value

	// Handle arrays specially - generate pointer + count
	if schema.Type.Is("array") {
		g.generateArrayField(fieldName, schema)
		return
	}

	// Handle nested objects
	if schema.Type.Is("object") {
		// Anonymous nested object - use void* for now
		g.out.WriteString(fmt.Sprintf("    void *%s;\n", fieldName))
		return
	}

	cType := g.openAPITypeToCType(propRef)
	g.out.WriteString(fmt.Sprintf("    %s %s;\n", cType, fieldName))
}

func (g *Generator) generateArrayField(fieldName string, schema *openapi3.Schema) {
	var itemType string

	if schema.Items != nil {
		if schema.Items.Ref != "" {
			// Array of referenced type
			itemType = g.resolveRefType(schema.Items.Ref)
		} else if schema.Items.Value != nil {
			itemType = g.openAPITypeToCType(schema.Items)
		} else {
			itemType = "void"
		}
	} else {
		itemType = "void"
	}

	// Generate pointer + count fields
	// Count field name follows the same style as the array field
	countFieldName := g.applyStyle(fieldName+"_count", g.fieldStyle)
	g.out.WriteString(fmt.Sprintf("    %s *%s;\n", itemType, fieldName))
	g.out.WriteString(fmt.Sprintf("    size_t %s;\n", countFieldName))
}

func (g *Generator) resolveRefType(ref string) string {
	// Extract schema name from $ref like "#/components/schemas/Competitor"
	parts := strings.Split(ref, "/")
	if len(parts) > 0 {
		schemaName := parts[len(parts)-1]
		return g.applyStyle(schemaName, g.structStyle)
	}
	return "void"
}

func (g *Generator) openAPITypeToCType(schemaRef *openapi3.SchemaRef) string {
	// Check for $ref first
	if schemaRef.Ref != "" {
		return g.resolveRefType(schemaRef.Ref) + "*"
	}

	schema := schemaRef.Value
	if schema == nil {
		return "void*"
	}

	nullable := schema.Nullable

	switch {
	case schema.Type.Is("string"):
		return "char*"

	case schema.Type.Is("integer"):
		baseType := "int64_t"
		switch schema.Format {
		case "int32":
			baseType = "int32_t"
		case "int64":
			baseType = "int64_t"
		}
		if nullable {
			return baseType + "*"
		}
		return baseType

	case schema.Type.Is("number"):
		baseType := "double"
		if schema.Format == "float" {
			baseType = "float"
		}
		if nullable {
			return baseType + "*"
		}
		return baseType

	case schema.Type.Is("boolean"):
		if nullable {
			return "bool*"
		}
		return "bool"

	case schema.Type.Is("array"):
		// For inline array types (not as struct fields), return pointer
		if schema.Items != nil {
			itemType := g.openAPITypeToCType(schema.Items)
			return itemType + "*"
		}
		return "void*"

	case schema.Type.Is("object"):
		return "void*"

	default:
		return "void*"
	}
}

func (g *Generator) generateHandlers() {
	if g.doc.Paths == nil {
		return
	}

	pathList := make([]string, 0)
	for path := range g.doc.Paths.Map() {
		pathList = append(pathList, path)
	}
	sort.Strings(pathList)

	for _, path := range pathList {
		pathItem := g.doc.Paths.Value(path)
		if pathItem == nil {
			continue
		}

		operations := []struct {
			method string
			op     *openapi3.Operation
		}{
			{"get", pathItem.Get},
			{"post", pathItem.Post},
			{"put", pathItem.Put},
			{"delete", pathItem.Delete},
			{"patch", pathItem.Patch},
		}

		for _, item := range operations {
			if item.op == nil {
				continue
			}
			g.generateHandler(item.method, path, item.op)
		}
	}
	g.out.WriteString("\n")
}

func (g *Generator) generateHandler(method, path string, op *openapi3.Operation) {
	// Summary as comment
	if op.Summary != "" {
		g.out.WriteString(fmt.Sprintf("// %s %s - %s\n", strings.ToUpper(method), path, op.Summary))
	} else {
		g.out.WriteString(fmt.Sprintf("// %s %s\n", strings.ToUpper(method), path))
	}

	funcName := g.operationToFuncName(method, path, op)
	g.out.WriteString(fmt.Sprintf("void %s(Req *req, Res *res);\n\n", funcName))
}

func (g *Generator) operationToFuncName(method, path string, op *openapi3.Operation) string {
	var name string

	if op.OperationID != "" {
		name = op.OperationID
	} else {
		// Generate from method + path
		pathPart := strings.ReplaceAll(path, "/", "_")
		pathPart = strings.ReplaceAll(pathPart, "{", "by_")
		pathPart = strings.ReplaceAll(pathPart, "}", "")
		pathPart = strings.Trim(pathPart, "_")
		name = method + "_" + pathPart
	}

	return g.applyStyle(name, g.funcStyle)
}

// Helper functions

func (g *Generator) sortedSchemaNames() []string {
	names := make([]string, 0, len(g.doc.Components.Schemas))
	for name := range g.doc.Components.Schemas {
		names = append(names, name)
	}
	sort.Strings(names)
	return names
}

func (g *Generator) sortedPropertyNames(props openapi3.Schemas) []string {
	names := make([]string, 0, len(props))
	for name := range props {
		names = append(names, name)
	}
	sort.Strings(names)
	return names
}

func (g *Generator) applyStyle(s string, style NamingStyle) string {
	switch style {
	case SnakeCase:
		return toSnakeCase(s)
	case CamelCase:
		return toCamelCase(s)
	case PascalCase:
		return toPascalCase(s)
	default:
		return s
	}
}

func toPascalCase(s string) string {
	parts := splitIdentifier(s)
	for i := range parts {
		if len(parts[i]) > 0 {
			parts[i] = strings.ToUpper(parts[i][:1]) + strings.ToLower(parts[i][1:])
		}
	}
	return strings.Join(parts, "")
}

func toCamelCase(s string) string {
	pascal := toPascalCase(s)
	if len(pascal) == 0 {
		return pascal
	}
	return strings.ToLower(pascal[:1]) + pascal[1:]
}

func toSnakeCase(s string) string {
	var result strings.Builder
	prevLower := false

	for i, r := range s {
		isUpper := r >= 'A' && r <= 'Z'
		isSeparator := r == '-' || r == '_' || r == ' '

		if isSeparator {
			if result.Len() > 0 {
				result.WriteRune('_')
			}
			prevLower = false
			continue
		}

		if i > 0 && isUpper && prevLower {
			result.WriteRune('_')
		}

		result.WriteRune(r)
		prevLower = !isUpper
	}

	return strings.ToLower(result.String())
}

func splitIdentifier(s string) []string {
	var parts []string
	var current strings.Builder

	for i, r := range s {
		isSeparator := r == '-' || r == '_' || r == ' '
		isUpper := r >= 'A' && r <= 'Z'

		if isSeparator {
			if current.Len() > 0 {
				parts = append(parts, current.String())
				current.Reset()
			}
			continue
		}

		// Start new part on uppercase after lowercase
		if i > 0 && isUpper && current.Len() > 0 {
			lastChar := []rune(current.String())[current.Len()-1]
			if lastChar >= 'a' && lastChar <= 'z' {
				parts = append(parts, current.String())
				current.Reset()
			}
		}

		current.WriteRune(r)
	}

	if current.Len() > 0 {
		parts = append(parts, current.String())
	}

	return parts
}

func wrapComment(s string) string {
	// Replace newlines with space for single-line comment
	s = strings.ReplaceAll(s, "\n", " ")
	s = strings.ReplaceAll(s, "\r", "")
	return strings.TrimSpace(s)
}
