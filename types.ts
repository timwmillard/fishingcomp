export type Json =
  | string
  | number
  | boolean
  | null
  | { [key: string]: Json }
  | Json[]

export interface Database {
  graphql_public: {
    Tables: {
      [_ in never]: never
    }
    Views: {
      [_ in never]: never
    }
    Functions: {
      graphql: {
        Args: {
          operationName?: string
          query?: string
          variables?: Json
          extensions?: Json
        }
        Returns: Json
      }
    }
    Enums: {
      [_ in never]: never
    }
    CompositeTypes: {
      [_ in never]: never
    }
  }
  public: {
    Tables: {
      catch_photo: {
        Row: {
          catch_id: number | null
          id: number
          is_feature: string | null
          storage_object_id: string
        }
        Insert: {
          catch_id?: number | null
          id?: never
          is_feature?: string | null
          storage_object_id: string
        }
        Update: {
          catch_id?: number | null
          id?: never
          is_feature?: string | null
          storage_object_id?: string
        }
      }
    }
    Views: {
      boats: {
        Row: {
          boat_rego: string | null
          event_id: number | null
          id: number | null
          name: string | null
          team_no: string | null
        }
        Insert: {
          boat_rego?: string | null
          event_id?: number | null
          id?: number | null
          name?: string | null
          team_no?: string | null
        }
        Update: {
          boat_rego?: string | null
          event_id?: number | null
          id?: number | null
          name?: string | null
          team_no?: string | null
        }
      }
      catch_reviews: {
        Row: {
          bait: string | null
          catch_id: number | null
          caught_at: string | null
          competitor_id: number | null
          event_id: number | null
          id: number | null
          location: unknown | null
          location_name: string | null
          photo_url: string | null
          reviewed_at: string | null
          reviewer_comment: string | null
          reviewer_user_id: string | null
          size: number | null
          species_id: number | null
          status: "submitted" | "approved" | "rejected" | null
          submitted_at: string | null
          submitter_user_id: string | null
        }
        Insert: {
          bait?: string | null
          catch_id?: number | null
          caught_at?: string | null
          competitor_id?: number | null
          event_id?: number | null
          id?: number | null
          location?: unknown | null
          location_name?: string | null
          photo_url?: string | null
          reviewed_at?: string | null
          reviewer_comment?: string | null
          reviewer_user_id?: string | null
          size?: number | null
          species_id?: number | null
          status?: "submitted" | "approved" | "rejected" | null
          submitted_at?: string | null
          submitter_user_id?: string | null
        }
        Update: {
          bait?: string | null
          catch_id?: number | null
          caught_at?: string | null
          competitor_id?: number | null
          event_id?: number | null
          id?: number | null
          location?: unknown | null
          location_name?: string | null
          photo_url?: string | null
          reviewed_at?: string | null
          reviewer_comment?: string | null
          reviewer_user_id?: string | null
          size?: number | null
          species_id?: number | null
          status?: "submitted" | "approved" | "rejected" | null
          submitted_at?: string | null
          submitter_user_id?: string | null
        }
      }
      catches: {
        Row: {
          bait: string | null
          caught_at: string | null
          competitor_id: number | null
          event_id: number | null
          id: number | null
          location: unknown | null
          location_name: string | null
          photo_url: string | null
          size: number | null
          species_id: number | null
        }
        Insert: {
          bait?: string | null
          caught_at?: string | null
          competitor_id?: number | null
          event_id?: number | null
          id?: number | null
          location?: unknown | null
          location_name?: string | null
          photo_url?: string | null
          size?: number | null
          species_id?: number | null
        }
        Update: {
          bait?: string | null
          caught_at?: string | null
          competitor_id?: number | null
          event_id?: number | null
          id?: number | null
          location?: unknown | null
          location_name?: string | null
          photo_url?: string | null
          size?: number | null
          species_id?: number | null
        }
      }
      competitors: {
        Row: {
          address1: string | null
          address2: string | null
          competitor_no: string | null
          email: string | null
          event_id: number | null
          first_name: string | null
          id: number | null
          last_name: string | null
          mobile: string | null
          person_id: number | null
          phone: string | null
          postcode: string | null
          state: string | null
          suburb: string | null
          team_id: number | null
        }
        Insert: {
          address1?: string | null
          address2?: string | null
          competitor_no?: string | null
          email?: string | null
          event_id?: number | null
          first_name?: string | null
          id?: number | null
          last_name?: string | null
          mobile?: string | null
          person_id?: number | null
          phone?: string | null
          postcode?: string | null
          state?: string | null
          suburb?: string | null
          team_id?: number | null
        }
        Update: {
          address1?: string | null
          address2?: string | null
          competitor_no?: string | null
          email?: string | null
          event_id?: number | null
          first_name?: string | null
          id?: number | null
          last_name?: string | null
          mobile?: string | null
          person_id?: number | null
          phone?: string | null
          postcode?: string | null
          state?: string | null
          suburb?: string | null
          team_id?: number | null
        }
      }
      events: {
        Row: {
          club_id: number | null
          end_timestamp: string | null
          id: number | null
          location: string | null
          name: string | null
          settings: Json | null
          slug: string | null
          start_timestamp: string | null
        }
        Insert: {
          club_id?: number | null
          end_timestamp?: string | null
          id?: number | null
          location?: string | null
          name?: string | null
          settings?: Json | null
          slug?: string | null
          start_timestamp?: string | null
        }
        Update: {
          club_id?: number | null
          end_timestamp?: string | null
          id?: number | null
          location?: string | null
          name?: string | null
          settings?: Json | null
          slug?: string | null
          start_timestamp?: string | null
        }
      }
      species: {
        Row: {
          common_names: string[] | null
          id: number | null
          name: string | null
          native: boolean | null
          photo_url: string | null
          scientific_name: string | null
          slug: string | null
        }
        Insert: {
          common_names?: string[] | null
          id?: number | null
          name?: string | null
          native?: boolean | null
          photo_url?: string | null
          scientific_name?: string | null
          slug?: string | null
        }
        Update: {
          common_names?: string[] | null
          id?: number | null
          name?: string | null
          native?: boolean | null
          photo_url?: string | null
          scientific_name?: string | null
          slug?: string | null
        }
      }
    }
    Functions: {
      [_ in never]: never
    }
    Enums: {
      [_ in never]: never
    }
    CompositeTypes: {
      [_ in never]: never
    }
  }
  storage: {
    Tables: {
      buckets: {
        Row: {
          allowed_mime_types: string[] | null
          avif_autodetection: boolean | null
          created_at: string | null
          file_size_limit: number | null
          id: string
          name: string
          owner: string | null
          public: boolean | null
          updated_at: string | null
        }
        Insert: {
          allowed_mime_types?: string[] | null
          avif_autodetection?: boolean | null
          created_at?: string | null
          file_size_limit?: number | null
          id: string
          name: string
          owner?: string | null
          public?: boolean | null
          updated_at?: string | null
        }
        Update: {
          allowed_mime_types?: string[] | null
          avif_autodetection?: boolean | null
          created_at?: string | null
          file_size_limit?: number | null
          id?: string
          name?: string
          owner?: string | null
          public?: boolean | null
          updated_at?: string | null
        }
      }
      migrations: {
        Row: {
          executed_at: string | null
          hash: string
          id: number
          name: string
        }
        Insert: {
          executed_at?: string | null
          hash: string
          id: number
          name: string
        }
        Update: {
          executed_at?: string | null
          hash?: string
          id?: number
          name?: string
        }
      }
      objects: {
        Row: {
          bucket_id: string | null
          created_at: string | null
          id: string
          last_accessed_at: string | null
          metadata: Json | null
          name: string | null
          owner: string | null
          path_tokens: string[] | null
          updated_at: string | null
        }
        Insert: {
          bucket_id?: string | null
          created_at?: string | null
          id?: string
          last_accessed_at?: string | null
          metadata?: Json | null
          name?: string | null
          owner?: string | null
          path_tokens?: string[] | null
          updated_at?: string | null
        }
        Update: {
          bucket_id?: string | null
          created_at?: string | null
          id?: string
          last_accessed_at?: string | null
          metadata?: Json | null
          name?: string | null
          owner?: string | null
          path_tokens?: string[] | null
          updated_at?: string | null
        }
      }
    }
    Views: {
      [_ in never]: never
    }
    Functions: {
      extension: {
        Args: {
          name: string
        }
        Returns: string
      }
      filename: {
        Args: {
          name: string
        }
        Returns: string
      }
      foldername: {
        Args: {
          name: string
        }
        Returns: string[]
      }
      get_size_by_bucket: {
        Args: Record<PropertyKey, never>
        Returns: {
          size: number
          bucket_id: string
        }[]
      }
      search: {
        Args: {
          prefix: string
          bucketname: string
          limits?: number
          levels?: number
          offsets?: number
          search?: string
          sortcolumn?: string
          sortorder?: string
        }
        Returns: {
          name: string
          id: string
          updated_at: string
          created_at: string
          last_accessed_at: string
          metadata: Json
        }[]
      }
    }
    Enums: {
      [_ in never]: never
    }
    CompositeTypes: {
      [_ in never]: never
    }
  }
}

