<?php
session_start();
if(!isset($_SESSION['logged'])) {
	header("Location: index.php");
	exit;
}
require_once("db/db.php");
$report = array();
try {
	$db = new Database(DB_SERVER, DB_USER, DB_PASS, DB_NAME);
	$report = $db->getReport();
	$db->close();
} catch (DBException $e) {
	echo $e->getMessage();
}
$giorni_sett = array("Domenica",
					 "Luned&igrave;", 
					 "Marted&igrave;",
					 "Mercoled&igrave;",
					 "Gioved&igrave;",
					 "Venerd&igrave;",
					 "Sabato");
?>
<!DOCTYPE html>
<html lang="it-IT">
<head>
	<meta charset="UTF-8">
	<title>Amministrazione</title>
	<script src="//ajax.googleapis.com/ajax/libs/jquery/1.8.3/jquery.min.js"></script>
	<style type="text/css">
		#button_div { overflow:hidden; }
		#button_div > div { float:left; }
		table, table td, table th { border:1px solid black; }
		.hidden-row { display:none; }
		table { border-collapse:collapse; }
		th, td { padding:10px; }
		.ip-wrapper { position:relative; }
		.ip-wrapper button { width:100%; }
		.ip-wrapper a
		{
			text-decoration:none;
			color:inherit;
			border-bottom: 1px dashed black;
			cursor:pointer;
		}
		.overlay
		{
			padding:5px;
			background-color:rgba(150, 150, 150, .8);
			border:1px solid black;
			border-radius:10px;
			position:absolute;
			top:-17px;
			left:115%;
			border:1px solid gray;
			width:182px;
			display:none;
		}
		.overlay:after
		{
			width: 0px;
			height: 0px;
			border-style: solid;
			border-width: 5px 10px 5px 0;
			border-color: transparent #888 transparent transparent;
			content:"";
			position:absolute;
			top:21px;
			left:-10px;
		}
		.overlay button { display:inline-block; width:90px; }
		.overlay button:not(:last-child) { margin-right:2px; }
		.reverse_lookup
		{
			font-family:"Courier New";
			font-size:15px;
			text-align:center;
			font-weight:bold;
			text-decoration:underline;
		}
		.flag-container { text-align:center; }
		.flag {
			display:inline-block;
			width:18px;
			height:12px;
		}
		.en { background:url("../images/english.jpg") no-repeat; }
		.it { background:url("../images/italian.jpg") no-repeat; }
	</style>
	<script>
	var onOverlay = 0;
	$(function(){
		$("table").on("mouseover mouseout click", ".ip-wrapper", function(e){
			var target = $(e.target), overlay;
			if(target.is("a"))
			{
				overlay = target.next();
				if(e.type == "mouseover") {
					overlay.fadeIn("medium");
				} else if(e.type == "mouseout"){
					setTimeout(function(){
						if(!onOverlay)
							overlay.fadeOut("medium");
					}, 500);
				} else if(e.type == "click"){
					target.closest("tr").next().toggle();
					return false;
				}
			}
			else if(target.is("button"))
			{
				if(e.type == "mouseover") {
					onOverlay = 1;
				} else if(e.type == "mouseout"){
					onOverlay = 0;
					overlay = target.parent();
					setTimeout(function(){
						if(!onOverlay)
							overlay.fadeOut("medium");
					}, 300);
				} else if(e.type == "click") {
					
					if(target.data("cmd") == "show_whois") {
						target.closest("tr").next().toggle();
						return;
					}
					
					var param = {
						cmd:target.data("cmd")
					};
					var ipWrapper = target.closest(".ip-wrapper");
					if(param.cmd == "delete_single_ip")
						param.param = ipWrapper.children("input[type=hidden]").eq(0).val();
					else if(param.cmd == "delete_all_from_ip")
						param.param = ipWrapper.children("a[href=#]").eq(0).html();
					$.post("execute.php", param, function(data){
						var obj = JSON.parse(data);
						if(obj.success == 0)
							alert("Errore: " + obj.reason);
						else
							$("#refresh_form").trigger("submit");
					});
				}
			}
		});
		$("#clear_form").on("submit", function(){
			if(!confirm("Sei sicuro?"))
				return false;
		});
		$("#dump").click(function(){
			$.post("execute.php", { cmd:"dump" }, function(data){
				$("#refresh_form").trigger("submit");
			});
		});
	});
	</script>
</head>
<body>
	<div id="button_div">
		<div class="logout_div">
			<form id="logout_form" action="logout.php" method="post">
				<input type="submit" value="Logout"/>				
			</form>
		</div>
		<div class="clear_div">
			<form id="clear_form" action="svuotaIp.php" method="post">
				<input type="submit" value="Svuota"/>				
			</form>
		</div>
		<div class="refresh_div">
			<form id="refresh_form" action="admin.php" method="post">
				<input type="submit" value="Aggiorna"/>				
			</form>
		</div>
		<button id="dump">Dump</button>
		<a href="handleDumps.php">&dash; Gestione Dump &dash;</a>
	</div>
	<br/>
	<?php if(empty($report)) { ?>
	<h3>Nessun IP</h3>
	<?php } else { ?>
	<table id="table">
		<thead>
			<tr>
				<th>IP</th>
				<th>Data/Ora</th>
				<th>Lang</th>
				<th width="40%">User Agent</th>
				<!-- <th>Referer</th> -->
				<th>Location 1</th>
				<th>Location 2</th>
				<th>Location 3</th>
			</tr>
		</thead>
		<tbody>
			<?php
			foreach($report as $record)
			{
				echo "<tr>";
				echo "<td>";
				echo "<div class='ip-wrapper'>";
				echo "<input type='hidden' name='key' value='" . $record['key'] . "'>";
				echo "<a href='#'>" . $record['ip'] . "</a>";
				echo "<div class='overlay'>";
				echo "<button data-cmd='delete_single_ip'>Cancella singolo</button>";
				echo "<button data-cmd='delete_all_from_ip'>Cancella tutti</button>";
				//echo "<button data-cmd='show_whois'>WhoIs</button>";
				echo "</div>";
				echo "</div>";
				echo "</td>";
				$str = $giorni_sett[date('w', $record['timestamp'])] . " " . date('d/m/Y - H:i:s', $record['timestamp']);
				echo "<td>$str</td>";
				echo "<td><div class='flag-container'><div class='flag " . $record['lang'] . "'></div></td>";
				echo "<td>" . $record['ua'] . "</td>";
				// echo "<td>" . $record['referer'] . "</td>";
				$info1 = json_decode($record['info1'], true);
				$info2 = json_decode($record['info2'], true);
				$info3 = json_decode($record['info3'], true);
				echo "<td>" . $info1['geoplugin_city'] . " - " . $info1['geoplugin_region'] . " - " . $info1['geoplugin_countryName'] . "</td>";
				echo "<td>" . $info2['cityName'] . " - " . $info2['regionName'] . " - " . $info2['countryName'] . "</td>";
				echo "<td>" . $info3['city'] . " - " . $info3['region'] . " - " . $info3['country'] . "</td>";
				echo "</tr>";
				
				echo "<tr class='hidden-row'>";
				echo "<td colspan='8'>";
				echo "<div class='reverse_lookup'>" . $record['reverse_lookup'] . "</div>";
				echo "<div class='whois'><pre>" . $record['whois'] . "</pre></div>";
				echo "</td>";
				echo "</tr>";
			}
			?>
		</tbody>
	</table>
	<?php } ?>
</body>
</html>