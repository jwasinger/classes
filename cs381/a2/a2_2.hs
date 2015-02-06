data Cmd = Pen Mode
         | MoveTo Int Int
         | Seq Cmd Cmd

data Mode = Up | Down

type State = (Mode, Int, Int)
type Line = (Int, Int, Int, Int)
type Lines = [Line]

semS :: Cmd -> State -> (State, Lines)
semS (Pen Up) (m, x, y)   = ((Up, x, y), [])
semS (Pen Down) (m, x, y) = ((Down, x, y), [])
semS (MoveTo x y) (m, x2, y2) = ((m, x2, y2), [(x, y, x2, y2)])
semS (Seq c c') (m, x, y) = (fst (semS c' (m, x, y)) , [(x,y,x,y)]++(snd (semS c' (m, x, y)) ))
-- sem' :: Cmd -> Lines

sem' :: Cmd -> Lines
sem' c = snd (semS c (Up, 0, 0))
