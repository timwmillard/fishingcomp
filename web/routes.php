<?php

$routes = [
    [ "method" => "GET", "path" => "/", "route" => "index.php", ],
    [
        "/" => "index.php",
        "/about" => "about.php",
        "/contact" => "contact.php",
        "/competitor/{id}/results" => "competitor/results.php",
    ],
];

?>
