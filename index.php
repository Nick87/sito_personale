<!DOCTYPE HTML>
<html>
<head>
	<meta charset="UTF-8">
	<meta name="viewport" content="width=device-width" />
	<meta name="description" content="Nicola Alessandro Domingo - Sito Web Personale" />
	<meta name="keywords" content="Nicola Alessandro Domingo, Sito Web Personale, Curriculum, Progetti, Contatti, Resume, Projects, Contact Me" />
	<meta name="author" content="Nicola Alessandro Domingo" />
	<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
	<title>Nicola Alessandro Domingo | Sito Web Personale</title>
	<link rel="canonical" href="http://www.nicoladomingo.it" />
	<link href='//fonts.googleapis.com/css?family=Lora' rel='stylesheet' type='text/css'>
	<link href='//fonts.googleapis.com/css?family=Oswald' rel='stylesheet' type='text/css'>
	<link rel="stylesheet" type="text/css" href="css/style.css" media="all" />
	<link rel="stylesheet" type="text/css" href="css/fancybox.css" media="all" />
	<script type="text/javascript">
        var _gaq = _gaq || [];
        _gaq.push(['_setAccount', 'UA-35666765-1']);
        _gaq.push(['_trackPageview']);
        (function() {
            var ga = document.createElement('script'); ga.type = 'text/javascript'; ga.async = true;
            ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';
            var s = document.getElementsByTagName('script')[0]; s.parentNode.insertBefore(ga, s);
        })();
    </script>
</head>
<body>
	<div id="container">
		<header id="header">
			<section class="siteDescription">
				<h1 class="siteTitle">Nicola Alessandro Domingo</h1>
				<h3 class="siteSubtitle">Personal Web Site</h3>
				<div id="langSwitcher">
					<a href="#" data-lang-from="en" data-lang-to="it" class="flag italian"></a>
				</div>
			</section>
			<nav class="main-nav">
				<input type="checkbox" id="menu"/>
				<label for="menu" onclick></label>
				<ul class="navigation">
					<li class="active"><a href="#cv">Resume</a>
					<li><a href="#workExperience">Work Experience</a>
					<li><a href="#projects">Personal&ndash;Academic Projects</a>
					<li><a href="#contacts">Contact Me</a>
				</ul>
			</nav>
		</header>
		<div class="innerContainer">
			<?php include "english.php"; ?>
		</div>
	</div>
	
	<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
	<script type="text/javascript" src="js/fancybox.js"></script>
	<script type="text/javascript" src="js/script.js"></script>
</body>
</html>