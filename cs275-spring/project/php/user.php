<?php 
require_once('connect.php');

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
}

class Order
{
	public $username;
	public $order_id;
	public $price;
	public $items;
	public $combos;
}

class Combo
{
	public $name;
	public $items;
}

class Item
{
	public $name;
	public $price;

	public function __construct($name, $price)
	{
		$this->name = $name;
		$this->price = $price;
	}
}

function get_all_items()
{
	$items = array();

	$query = mysql_query('SELECT * FROM Items');
	if(!$query)
	{
		echo "MySQL error: ".mysql_error();
		die();
	}

	while($row = mysql_fetch_array($query))
	{
		$items[] = new Item($row[0], $row[1]);
	}

	return $items;
}

function get_highest_order_id()
{
	$query = mysql_query('SELECT MAX(order_id) FROM Orders');
	if(!$query)
	{
		echo "error: ".mysql_error();
		die();
	}

	return mysql_fetch_array($query)[0];
}

function calculate_items_cost($items)
{
	$count = 0;
	for($i = 0; $i < count($items); $i++)
	{
		$count += $items[$i]->price;
	}

	return $count;
}

function add_user_order($username, $order)
{
	$new_order_id = get_highest_order_id() + 1;
	$new_order_cost = calculate_items_cost($order->items);
	$query = mysql_query('INSERT INTO Orders
						  VALUES ('.$new_order_cost.','.$new_order_id.','.$username.')');

	if(!$query)
	{
		echo "MySQL error: ".mysql_error();
		die();
	}
}


function get_user_orders($username)
{
	$orders_query = mysql_query('SELECT * FROM Orders
						   		 WHERE customer="'.$username.'"');

	$orders = array();

	if(!$orders_query)
	{
		echo "Error: ".mysql_error();
		die();
	}

	while($row = mysql_fetch_array($orders_query, MYSQL_NUM))
	{
		$order = new Order();
		$order->username = $row[2];
		$order->order_id = $row[1];
		$order->price = $row[0];

		$order->items = array();

		$items_query = mysql_query('SELECT order_id, item_name, price
									FROM Items_Orders
									INNER JOIN Items ON Items_Orders.item_name = Items.name
									WHERE order_id = "'.$order->order_id.'"
									');
		if(!$items_query)
		{
			echo 'MySQL error: '.mysql_error();
			die();
		}

		//add each item to the order
		while($items_row = mysql_fetch_array($items_query))
		{
			$item = new Item($items_row[1], $items_row[2]);
			$order->items[] = $item;
		}

		$orders[] = $order;
	}

	return $orders;
}

?>