<?php
include "../www/db.php";
$SQL = "SELECT text FROM vikingtech.robot WHERE complete = false ORDER BY id LIMIT 1";
$ans =  pg_fetch_all(pg_query($db, $SQL));
echo $ans[0]['text'];
?>