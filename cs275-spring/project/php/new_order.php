<?php 

// function validate_order()
// {
// 	$items = get_all_items();
// 	for($i=0; $i<count($items); $i++)
// 	{
// 		if(!$_POST[$items[$i]->name.'_qty'] == 1)
// 		{
// 			return false;
// 		}

// 		if($_POST[$items[$i]->name.'_qty'] < 0)
// 		{
// 			return false;
// 		}
// 	}

// 	return true;
// }

require_once('connect.php');
include 'user.php';

if($_SERVER['REQUEST_METHOD'] === 'GET')
{
	include '../html/new_order.html';
}
else
{
	$order = new Order();
	$order->username = 'wasingej'; //TODO: allow other usernames to be used!
	$order->order_id = 
	add_user_order();
}

?>