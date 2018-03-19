<?php
session_start();
if(isset($_SESSION['logged'])) {
	header("Location: admin.php");
	exit;
}
?>
<!DOCTYPE html>
<html>
<head>
	<meta charset="UTF-8" />
	<title>Amministrazione</title>
	<link rel="stylesheet" type="text/css" href="css/demo.css" />
	<link rel="stylesheet" type="text/css" href="css/style.css" />
</head>
<body>
	<div class="container">
		<section>				
			<div id="container_demo" >
				<a class="hiddenanchor" id="toregister"></a>
				<a class="hiddenanchor" id="tologin"></a>
				<div id="wrapper">
					<div id="login" class="animate form">
						<form method="post" action="login.php" autocomplete="on"> 
							<h1>Log in</h1> 
							<p> 
								<label for="password" class="youpasswd" data-icon="p"></label>
								<input id="password" name="password" required="required" type="password" placeholder="Inserisci la password" autofocus="autofocus"/> 
							</p>
							<!--
							<p class="keeplogin"> 
								<input type="checkbox" name="loginkeeping" id="loginkeeping" value="loginkeeping" /> 
								<label for="loginkeeping">Resta collegato</label>
							</p>
							-->
							<p class="login button"> 
								<input type="submit" style="letter-spacing:3px" value="Login" /> 
							</p>
							<p class="change_link">
								<a href="http://www.nicoladomingo.it">Torna al sito</a>
							</p>
						</form>
					</div>
				</div>
			</div>  
		</section>
	</div>
</body>
</html>