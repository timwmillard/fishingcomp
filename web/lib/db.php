<?php

function connect() {
    $dbconn = pg_connect("dbname=fishingcomp"); 
    return $dbconn; 
}

?>

