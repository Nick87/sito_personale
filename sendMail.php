<?php
$to = "nicola.domingo@gmail.com";
$subject = "Messaggio da www.nicoladomingo.it";
$body = trim(strip_tags(stripslashes($_POST["message"])));
$headers = "From: Messaggio da www.nicoladomingo.it";

if($body == "") {
	echo json_encode(array("ok" => "false", "message" => "Write a message"));
	exit;
}

if(@mail($to, $subject, $body, $headers)) {
	echo json_encode(array("ok" => "true", "message" => "Email sent successfully"));
} else {
	echo json_encode(array("ok" => "false", "message" => "Error while sending email"));
}
