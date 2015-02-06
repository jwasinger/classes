type Prog = [Cmd]
type Stack = [Int]

data Cmd = LD Int
         | ADD 
         | MULT 
         | DUP
         deriving Show

type D = Maybe Stack -> Maybe Stack

-- evalStack :: Stack -> D
-- evalStack s = if ((length s) >= 2) then Valid s
--              else Error
                
semCmd :: Cmd -> D
semCmd (LD i) (Just s)        = Just (i:s)
semCmd (ADD)  (Just (x:y:xs)) = Just ((x+y):xs)
-- semCmd (ADD)  (Just s)        = Nothing

semCmd (MULT) (Just (x:y:xs)) = Just ((x*y):xs)
--semCmd (MULT) (Just s)        = Nothing

semCmd DUP    (Just (x:xs))   = Just ((x*2):xs)
semCmd c s = Nothing 

sem :: Prog -> D 
sem [] c = c 
sem (o:os) c = sem os (semCmd o c)