<?php
require 'lib/auth.php';
require 'lib/config.php';

$query = <<<SQL
    select *
    from fishing.competitor;
SQL;

$conn = pg_connect($DB);
$result = pg_query($conn, $query);
/* echo $data; */

include 'shared/header.php';
?>
    <h1>Murrabit Cod Challenge</h1>
    <table>
    <?php while ($line = pg_fetch_array($result, null, PGSQL_ASSOC)): ?>
        <tr>
        <?php foreach ($line as $col_value): ?>
            <td><?= $col_value ?></td>
        <?php endforeach ?>
        </tr>
    <?php endwhile ?> 
    </table>


<?php
include 'shared/footer.php'
?>

