<?php

$db_initiallized = false;
$db_connection = NULL;

function db_init()
{
	global $db_initiallized, $db_connection;

	if(!db_initiallized)
	{
		$db_connection = mysql_connect("mysql.cs.orst.edu", "cs275_wasingej", "7406");
		if(!$db_connection)
		{
			echo 'Connection failure!';
			exit();
		}

		$db_initiallized = true;
	}
}

db_init();

$db = mysql_select_db("cs275_wasingej", $db_connection);
if(!$db)
{
	echo 'Database failure!';
}

$query = 'SELECT * FROM User';
$result = mysql_query($query);

if(!$result)
{
	die('Fatal error(s): '.mysql_error());
}

include "../html/index.html";

?>

