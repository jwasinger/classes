<?php

// if(!isset($GLOBALS['db_initiallized']))
// {
// 	$GLOBALS['db_initiallized'] = false;
// }

// if(!isset($GLOBALS['db_connection']))
// {
// 	$GLOBALS['db_connection'] = null;	
// }

// $db_connection = null;

function db_init()
{
	if(!$GLOBALS['db_initiallized'])
	{
		$GLOBALS['db_connection'] = mysql_connect("mysql.cs.orst.edu", "cs275_wasingej", "7406");
		if(!$GLOBALS['db_connection'])
		{
			echo 'Connection failure!';
			exit();
		}

		$GLOBALS['db_initiallized'] = true;
	}
}

$db_connection = mysql_pconnect("mysql.cs.orst.edu", "cs275_wasingej", "7406");
if(!$db_connection)
{
	echo 'Connection failure!';
	exit();
}

$db = mysql_select_db("cs275_wasingej", $db_connection);
if(!$db)
{
	echo 'Database failure: '.mysql_error();
}

$query = 'SELECT * FROM Users';
$result = mysql_query($query);

if(!$result)
{
	die('Fatal error(s): '.mysql_error());
}

?>