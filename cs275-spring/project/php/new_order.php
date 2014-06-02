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

//get the items the user ordered based on values contained in the submitted POST form
function get_order_items()
{
	$items = array();

	for($i = 0; $i < $_POST['small_fries_qty']; $i++)
	{
		$items[] = new Item('small_fries', 1.5);
	}

	for($i = 0; $i < $_POST['small_drink_qty']; $i++)
	{
		$items[] = new Item('small_drink', 2.5);
	}

	return $items;
}

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
	$order->order_id = get_highest_order_id() + 1;
	$order->items = get_order_items();
	$order->price = calculate_items_cost($order->items);
	$order->combos = null;
	add_user_order('wasingej', $order);

	header('Location: ../php/user_home.php');
}

?>