<?php 

function get_user_wallet($username)
{
	$query = mysql_query('SELECT wallet_amt 
						  FROM Users
						  WHERE username="'.$username.'"');
	if(!$query)
	{
		echo "MySQL error!";
		return null;
	}
	else
	{
		return $query;
	}

	class Order 
	{
		public $username;
		
	}

	function get_user_orders($username)
	{

	}
} 

?>