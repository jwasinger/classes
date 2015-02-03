revmap :: (a -> b) -> [a] -> [b]
revmap f [] = [] 
revmap f (x:xs) = revmap f xs ++ [f x] 

revmap_fc :: (a -> b) -> [a] -> [b]
revmap_fc f = reverse . map f

my_last :: [a] -> a
my_last (x:xs) = last xs
my_last [x] = x
