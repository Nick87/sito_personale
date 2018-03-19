<?php
require_once('db/db_conf.php');
session_start();
if ($_POST['password'] == LOGIN_PSW) {
	$_SESSION['logged'] = 1;
	header("Location: admin.php");
} else {
	session_destroy();
	header("Location: index.php");
	
}