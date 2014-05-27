<?php 
	$username = _POST['username'];
	$password = _POST['password'];
	$password = md5($password);

	if(!$db)
	{
		echo 'Could not connect to \'Users\' database';
	}

	//see if the username already exists
	$query = 'SELECT * FROM Users
			  WHERE username=$username, password=$password;';
	
	$result = mysql_query($query, $db_connection);
	if(mysql_num_rows($result) != 0)
	{
		echo '<script language="javascript">
				 alert("account already exists") 
			  </script>';
	}
	else
	{
		$query = 'INSERT INTO Users
				  VALUES ($username, $password);';

		$result = mysql_query($query);

		if(!$result)
		{
			echo 'MySql error: '.mysql_error();
		}
		else
		{
			header('Location: index.php');
		}
	}
?>