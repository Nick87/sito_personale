<?php
session_start();
if(isset($_SESSION['logged']) and $_SESSION['logged'] == 1)
{
	require_once("dump.php");
	$dump = new Dump();
	$dump->clearDumps();
	header("Location: handleDumps.php");
}
else
{
	session_destroy();
	header("HTTP/1.1 403 Forbidden");
}