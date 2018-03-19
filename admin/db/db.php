<?php
require_once("db_conf.php");
require_once("ip2location.php");

class DBException extends Exception
{
	function __construct($message)
	{
		$this->message =  $message;
	}
}

class Database
{
	private $server = "";
	private $user = "";
	private $pass = "";
	private $db_name = "";
	private $conn = 0;
	private $query = 0;
	
	public $error = "";
	public $errno = 0;
	public $num_rows = 0;
	
	function __construct($server, $user, $pass, $db_name)
	{
		$this->server = $server;
		$this->user = $user;
		$this->pass = $pass;
		$this->db_name = $db_name;
		
		$this->conn = @new mysqli($server, $user, $pass, $db_name);
		if ($this->conn->connect_errno) {
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			throw new DBException($this->conn->connect_error);
		}
		$this->errno = 0;
		$this->error = "";
	}
	
	public function dump($tables = TABLE_IP)
	{
		if($tables == '*') {
			$tables = array();
			$result = $this->conn->query('SHOW TABLES');
			while($row = $result->fetch_row())
				$tables[] = $row[0];
		} else {
			$tables = is_array($tables) ? $tables : explode(',', $tables);
		}

		foreach($tables as $table)
		{
			$result = $this->conn->query('SELECT * FROM ' . $table);
			$num_fields = $result->field_count;

			$return = 'DROP TABLE ' . $table . ';';
			$row2 = $this->conn->query('SHOW CREATE TABLE ' . $table)->fetch_row();
			$return .= "\n\n" . $row2[1] . ";\n\n";

			for ($i = 0; $i < $num_fields; $i++) 
			{
				while($row = $result->fetch_row())
				{
					$return.= 'INSERT INTO ' . $table . ' VALUES(';
					for($j = 0; $j < $num_fields; $j++) 
					{
						$row[$j] = addslashes($row[$j]);
						$row[$j] = preg_replace("/\n/", "\\n", $row[$j]);
						if (isset($row[$j])) { $return .= '"' . $row[$j] . '"' ; } else { $return .= '""'; }
						if ($j < ($num_fields - 1)) { $return .= ','; }
					}
					$return.= ");\n";
				}
			}
		}
		$handle = fopen( 'backups/' . date('d.m.Y - H.i.s', time()) . '.sql', 'w+');
		fwrite($handle, $return);
		fclose($handle);
	}
	
	public function restoreDump($dump)
	{
		if(!file_exists("backups/" . $dump))
			throw new DBException("File inesistente (" . $dump . ")");
		$lines = file("backups/" . $dump);
		$this->execQuery($lines);
	}
	
	/*public function executeQuery($query)
	{
		if($query == "")
			throw new DBException("Empty input");
		$lines = preg_split('/\r\n|\r|\n/', $query);
		$this->execQuery($lines);
	}*/
	
	private function execQuery($lines)
	{
		$command = "";
		$found = false;
		foreach ($lines as $line)
		{
			$line = preg_replace("/\\n/", "", $line);
			if (substr($line, 0, 2) == '--' || $line == '') continue;
			$command .= $line;
			if (substr(trim($line), -1, 1) == ';') {
				if($this->conn->query($command) !== TRUE)
					throw new DBException($this->conn->error);
				$command = "";
				$found = true;
			}
		}
		if(!$found)
			throw new DBException("No query provided.");
	}
	
	public function getReport($ip = "")
	{
		$stmt = "";
		$res = array();
		$i = 0;

		if($ip == ""){		
			$stmt = $this->conn->prepare("SELECT * FROM " . TABLE_IP . " ORDER BY timestamp DESC;");
		} else {
			$stmt = $this->conn->prepare("SELECT * FROM " . TABLE_IP ." WHERE ip = ? ORDER BY timestamp DESC;");
			$stmt->bind_param("s", $ip);
		}

		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$stmt->store_result();
		$this->num_rows = $stmt->num_rows;
		$stmt->bind_result($key, $ipAddress, $timestamp, $ua, $info1, $info2, $info3, $whois, $reverse_lookup, $lang, $referer);

		while ($stmt->fetch()) {
			$res[$i]['key'] = $key;
			$res[$i]['ip'] = $ipAddress;
			$res[$i]['timestamp'] = $timestamp;
			$res[$i]['ua'] = $ua;
			$res[$i]['info1'] = $info1;
			$res[$i]['info2'] = $info2;
			$res[$i]['info3'] = $info3;
			$res[$i]['whois'] = $whois;
			$res[$i]['reverse_lookup'] = $reverse_lookup;
			$res[$i]['lang'] = $lang;
			$res[$i]['referer'] = $referer;
			$i++;
		}

		$stmt->close();
		return $res;
	}
	
	public function clear()
	{
		$stmt = $this->conn->prepare("TRUNCATE " . TABLE_IP . ";");
		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$this->num_rows = $this->conn->affected_rows;
	}
	
	public function insertIp($ip, $lang, $timestamp, $ua, $referer)
	{
		$info1 = json_encode(unserialize(file_get_contents('http://www.geoplugin.net/php.gp?ip=' . $ip)));
		$ipLite = new ip2location_lite;
		$ipLite->setKey('c1262199ed1f7c8857d7d8f7832e5bb67ff657b6890179cfa1ecfc9988077b6c');
		$info2 = json_encode($ipLite->getCity($ip));
		$info3 = json_encode(get_meta_tags('http://www.geobytes.com/IpLocator.htm?GetLocation&template=php3.txt&IpAddress=' . $ip));
		$whois = $this->whois($ip);
		$reverse_lookup = gethostbyaddr($ip);
		$stmt = $this->conn->prepare("INSERT INTO " . TABLE_IP . " (ip, timestamp, user_agent, info1, info2, info3, whois, reverse_lookup, lang, referer) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?);");
		$stmt->bind_param('sissssssss', $ip, $timestamp, $ua, $info1, $info2, $info3, $whois, $reverse_lookup, $lang, $referer);
		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$stmt->close();
		$this->num_rows = 1;
		
		$info1 = json_decode($info1, true);
		$info2 = json_decode($info2, true);
		$info3 = json_decode($info3, true);
		$giorni_sett = array("Domenica",
					 "Lunedì", 
					 "Martedì",
					 "Mercoledì",
					 "Giovedì",
					 "Venerdì",
					 "Sabato");
		
		$to = "nicola.domingo@gmail.com";
		$subject = "Visita a www.nicoladomingo.it";
		$body = "Visita a www.nicoladomingo.it\n\n";
		$body .= "IP: " . $ip . "\n";
		$body .= "Lang: " . $lang . "\n";
		$body .= "Timestamp: " . $giorni_sett[date('w', $timestamp)] . " " . date('d/m/Y - H:i:s', $timestamp) . "\n";
		$body .= "User Agent: " . $ua . "\n";
		$body .= "Referer: " . $referer . "\n";
		$body .= "Reverse Lookup: " . $reverse_lookup . "\n";
		$body .= "Location1: " . $info1['geoplugin_city'] . " - " . $info1['geoplugin_region'] . " - " . $info1['geoplugin_countryName'] . "\n";
		$body .= "Location2: " . $info2['cityName'] . " - " . $info2['regionName'] . " - " . $info2['countryName'] . "\n";
		$body .= "Location3: " . $info3['city'] . " - " . $info3['region'] . " - " . $info3['country'] . "\n";
		$body .= "Whois:\n" . html_entity_decode($whois) . "\n";
		$headers = "";
		
		@mail($to, $subject, $body, $headers);
	}
	
	private function whois($ip)
	{
		$ch = curl_init("http://networking.ringofsaturn.com/Tools/whois.php");
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($ch, CURLOPT_REFERER, "http://networking.ringofsaturn.com/Tools/whois.php");
		curl_setopt($ch, CURLOPT_POSTFIELDS, "domain=" . $ip);
		$response = $this->getTextBetweenTags(curl_exec($ch), "pre");
		curl_close($ch);
		return $response;
	}
	
	private function getTextBetweenTags($string, $tagname)
	{
		$pattern = "/<$tagname>([\w\W]*?)<\/$tagname>/i";
		preg_match($pattern, $string, $matches);
		return trim($matches[1]);
	}
	
	public function deleteSingleIp($id)
	{
		$stmt = $this->conn->prepare("DELETE FROM " . TABLE_IP . " WHERE id = ?;");
		$stmt->bind_param('i', $id);
		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$stmt->close();
		$this->num_rows = $this->conn->affected_rows; // == 1 poiche' id e' chiave primaria
	}
	
	public function deleteAllFromIp($ip)
	{
		$stmt = $this->conn->prepare("DELETE FROM " . TABLE_IP . " WHERE ip = ?;");
		$stmt->bind_param('s', $ip);
		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$stmt->close();
		$this->num_rows = $this->conn->affected_rows;;
	}
	
	public function incrementCounter($link)
	{
		$stmt = $this->conn->prepare("SELECT COUNT(*) as counter FROM " . TABLE_COUNTER . " WHERE link = ?;");
		$stmt->bind_param('s', $link);
		if(!@$stmt->execute()){
			$this->errno = $this->conn->connect_errno;
			$this->error = $this->conn->connect_error;
			$stmt->close();
			throw new DBException($this->conn->connect_error);
		}
		$stmt->store_result();
		$this->num_rows = $stmt->num_rows;
		$stmt->bind_result($counter);
		$stmt->fetch();
		
		if($counter == 0)
		{
			$stmt = $this->conn->prepare("INSERT INTO " . TABLE_COUNTER . "(link) VALUES(?);");
			$stmt->bind_param('s', $link);
			if(!@$stmt->execute()){
				$this->errno = $this->conn->connect_errno;
				$this->error = $this->conn->connect_error;
				$stmt->close();
				throw new DBException($this->conn->connect_error);
			}
		}
		else
		{
			$stmt = $this->conn->prepare("SELECT counter FROM " . TABLE_COUNTER . " WHERE link = ?;");
			$stmt->bind_param('s', $link);
			if(!@$stmt->execute()){
				$this->errno = $this->conn->connect_errno;
				$this->error = $this->conn->connect_error;
				$stmt->close();
				throw new DBException($this->conn->connect_error);
			}
			
			$stmt->store_result();
			$this->num_rows = $stmt->num_rows;
			$stmt->bind_result($counter);
			$stmt->fetch();
			
			$counter++;
			$stmt = $this->conn->prepare("UPDATE " . TABLE_COUNTER . " SET counter = ? WHERE link = ?;");
			$stmt->bind_param('is', $counter, $link);
			
			if(!@$stmt->execute()){
				$this->errno = $this->conn->connect_errno;
				$this->error = $this->conn->connect_error;
				$stmt->close();
				throw new DBException($this->conn->connect_error);
			}
		}
		
		$stmt->close();
	}
	
	public function close()
	{
		if(!@$this->conn->close()) {
			$this->errno = $this->conn->errno;
			$this->error = $this->conn->error;
			throw new DBException($this->conn->error);
		}
		$this->errno = 0;
		$this->error = "";
	}
}