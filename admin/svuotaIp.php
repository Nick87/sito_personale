<?php
session_start();
if(isset($_SESSION['logged']) and $_SESSION['logged'] == 1)
{
	require_once("db/db.php");
	$report = array();
	$db = "";
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		$report = $db->clear();
		$db->close();
	} catch (DBException $e) {
		$db->close();
	}
	header("Location: admin.php");
}
else
{
	session_destroy();
	header("HTTP/1.1 403 Forbidden");
}