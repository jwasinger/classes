data Shape = X
           | TD Shape Shape
           | LR Shape Shape
           deriving Show

type BBox = (Int, Int)

bbox :: Shape -> BBox
bbox (X)      = (1,1)
bbox (TD s s') = (fst (bbox s) + fst(bbox s'),  snd(bbox s) + snd(bbox s'))
bbox (LR s s') = (fst (bbox s) + fst(bbox s'),  snd(bbox s) + snd(bbox s'))
