<?php
class Dump
{
	private $dumps = array();
	
	function __construct()
	{
		$dumps = $this->updateDumps();
	}
	
	private function updateDumps()
	{
		$i = 0;
		$this->dumps = array();
		if ($handle = opendir('backups'))
		{
			while (($entry = readdir($handle)) !== false)
			{
				if(!preg_match("/(.+)\.sql$/", $entry)) continue;
				$this->dumps[$i]['filename'] = $entry;
				$this->dumps[$i]['file_content'] = file_get_contents("backups/".$entry);
				$i++;
			}
			closedir($handle);
		}
	}
	
	public function getAllDumps()
	{
		return $this->dumps;
	}
	
	public function getNDumps()
	{
		return count($this->dumps);
	}
	
	public static function removeDump($dump)
	{
		if(!file_exists("backups/" . $dump))
			throw new Exception("File inesistente (" . $dump . ")");
		else
			unlink("backups/" . $dump);
	}
	
	public function clearDumps()
	{
		$this->dumps = array();
		if ($handle = opendir('backups'))
		{
			while (($entry = readdir($handle)) !== false) {
				if(preg_match("/(.+)\.sql$/", $entry))
					unlink("backups/".$entry);
			}
			closedir($handle);
		}
	}
}