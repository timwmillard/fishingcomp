<?php

define("INSERT_WORKER_SQL", <<<SQL
    insert into worker.river_job (state, max_attempts, kind, args, queue)
    values (
        'scheduled', 25, $1, -- kind
        jsonb_build_object($2), -- args
        $3 -- queue
    );
SQL);

function worker_send_email($tx, $email) {

    $result = pg_query($tx, INSERT_WORKER_SQL, $email);

    return $result;
}

?>
