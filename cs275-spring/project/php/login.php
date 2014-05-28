<?php

include "connect.php";

$GLOBALS['user_login_fail'] = false;

$username = mysql_real_escape_string($_POST['wasingej']);
$password = $_POST['password'];

//for now, store passwords in plain text
//$password = md5($password);

//see if the user entered the correct credentials
$login = mysql_query('SELECT * FROM users 								
					  WHERE (username="'.$username.'") AND (password="'.$password.'")');

if($login && mysql_num_rows($login) == 1)
{
	session_start();
	$_SESSION['username'] = $_POST['username'];
	header('Location: user_home.php');
}
else
{
	//$GLOBALS['user_login_fail'] = true;
	header('Location: ../index.php?user_login_fail=true');
}

?>