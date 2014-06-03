<?php
include "connect.php";
include "user.php";
$GLOBALS['user_login_fail'] = false;

$username = mysql_real_escape_string($_POST['username']);
$password = $_POST['password'];

//for now, store passwords in plain text
//$password = md5($password);

//see if the user entered the correct credentials
$login = mysql_query('SELECT * FROM Users 								
					  WHERE (username="'.$username.'") AND (password="'.$password.'")');

if($login && mysql_num_rows($login) == 1)
{
	session_start();
	
	$_SESSION['username'] = $_POST['username'];
	
	$wallet_amt = get_user_wallet($_POST['username']);
	if(!$wallet_amt)
	{
		echo "MySQL error";
		exit(1);
	}

	$_SESSION['wallet_amt'] = $wallet_amt;
	header('Location: user_home.php');
}
else
{
	header('Location: ../index.php?user_login_fail=true');
}

?>