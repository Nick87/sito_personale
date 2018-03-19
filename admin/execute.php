<?php
if(!isset($_POST['cmd'])) exit;

$cmd = $_POST['cmd'];
$db = "";

if($cmd == "insert") {
	if(!isset($_POST['lang'])){
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => "lang is null"));
	} else if($_POST['lang'] != "it" and $_POST['lang'] != "en"){
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => "unknown lang '" . $_POST['lang'] . "'"));
	} else {
		require_once("db/db.php");
		try {
			$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
			$ip = $_SERVER['REMOTE_ADDR'];
			if($ip != "::1" and $ip != "127.0.0.1" and strpos(strtoupper($_SERVER['HTTP_USER_AGENT']), "GOOGLEBOT") === false)
				$db->insertIp($ip, $_POST['lang'], $_SERVER['REQUEST_TIME'], $_SERVER['HTTP_USER_AGENT'], $_SERVER['HTTP_REFERER']);
			//$db->insertIp("130.25.3.188", $_POST['lang'], $_SERVER['REQUEST_TIME'], $_SERVER['HTTP_USER_AGENT'], $_SERVER['HTTP_REFERER']);
			$db->close();			
			echo json_encode(array("cmd" => $cmd, "success" => 1));
		} catch (DBException $e) {
			$db->close();
			echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
		}
	}
} else if($cmd == "delete_single_ip" or $cmd == "delete_all_from_ip") {
	session_start();
	if(!isset($_SESSION['logged']) or $_SESSION['logged'] != 1) {
		session_destroy();
		header("HTTP/1.1 401 Unauthorized");
		exit;
	}
	
	if(!isset($_POST['param'])) {
		header("HTTP/1.1 400 Bad Request");
		exit;
	}
	
	require_once("db/db.php");
	$param = $_POST['param'];
	
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		if($cmd == "delete_single_ip")
			$db->deleteSingleIp($param);
		else if($cmd == "delete_all_from_ip")
			$db->deleteAllFromIp($param);
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch (DBException $e) {
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else if($cmd == "dump") {
	require_once("db/db.php");
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		$db->dump();
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch (DBException $e) {
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else if($cmd == "delete-dump") {
	if(!isset($_POST['param'])){
		header("HTTP/1.1 400 Bad Request");
		exit;
	}
	
	require_once("dump.php");
	try {
		Dump::removeDump($_POST['param']);
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch(Exception $e) {
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else if($cmd == "restore-dump") {
	if(!isset($_POST['param'])){
		header("HTTP/1.1 400 Bad Request");
		exit;
	}
	
	require_once("db/db.php");
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		$db->restoreDump($_POST['param']);
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch (DBException $e) {
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else if($cmd == "execute-query") {
	if(!isset($_POST['param'])){
		header("HTTP/1.1 400 Bad Request");
		exit;
	}
	
	require_once("db/db.php");
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		$db->executeQuery($_POST['param']);
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch (DBException $e) {
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else if($cmd == "increment-counter") {
	if(!isset($_POST['param'])){
		header("HTTP/1.1 400 Bad Request");
		exit;
	}
	
	require_once("db/db.php");
	try {
		$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
		$db->incrementCounter($_POST['param']);
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 1));
	} catch (DBException $e) {
		$db->close();
		echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => $e->getMessage()));
	}
} else {
	echo json_encode(array("cmd" => $cmd, "success" => 0, "reason" => "unknown command '" . $_POST['cmd'] . "'"));
}