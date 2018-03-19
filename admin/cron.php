<?php
require_once("db/db.php");

try {
	$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
	$db->dump();
	$db->close();
} catch (DBException $e) {
	$db->close();
}