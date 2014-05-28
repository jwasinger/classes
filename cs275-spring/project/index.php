<?php
$_GLOBAL['db_initiallized'] = false;
$_GLOBAL['db_connection'] = null;

function db_init()
{

	if(!$_GLOBAL['db_initiallized'])
	{
		$_GLOBAL['db_connection'] = mysql_connect("mysql.cs.orst.edu", "cs275_wasingej", "7406");
		if(!$_GLOBAL['db_connection'])
		{
			echo 'Connection failure!';
			exit();
		}

		$_GLOBAL['db_initiallized'] = true;
	}
}

db_init();

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

include "./html/index.html";
?>

