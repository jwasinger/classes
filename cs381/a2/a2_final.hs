--Team Members:  Jared Wasinger, Justin Thrailkill

import System.IO

--Excercise #1

type Prog = [Cmd]
type Stack = [Int]

data Cmd = LD Int
         | ADD 
         | MULT 
         | DUP
         deriving Show

type D = Maybe Stack -> Maybe Stack

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

--
--Excercise #2
--

ppLines :: Lines -> IO ()
ppLines ls = withFile "lines.svg" WriteMode $ flip hPutStrLn $ toSVG ls

toSVG :: Lines -> String
toSVG ls =
  "<svg width='400' height='400' xmlns='http://www.w3.org/2000/svg'>\n" ++
  "<g transform='translate(0,400)'>\n<g transform='scale(1,-1)'>\n" ++
  concatMap lineSVG ls ++ "</g>\n</g>\n</svg>"

lineSVG :: Line -> String
lineSVG (x1,y1,x2,y2) =
  "<line stroke='black' stroke-width='2'" ++
  "  x1='" ++ show x1 ++
  "' y1='" ++ show y1 ++
  "' x2='" ++ show x2 ++
  "' y2='" ++ show y2 ++ "'/>\n"

--Our code below

data Cmd = Pen Mode
         | MoveTo Int Int
         | Seq Cmd Cmd
	 deriving Show

data Mode = Up | Down deriving Show

type State = (Mode, Int, Int)
type Line = (Int, Int, Int, Int)
type Lines = [Line]

stateGetX :: State -> Int 
stateGetX (a, x, y) = x

stateGetY :: State -> Int
stateGetY (a, x, y) = y

semS :: Cmd -> State -> (State, Lines)
semS (Pen Up) (m, x', y')   = ((Up, x', y'), [])
semS (Pen Down) (m, x', y') = ((Down, x', y'), [])

--
--As follows is our attempt to allow the pen to move to a different
--part of the screen without drawing (Pen Up).  The code has been
--commented out.  A little partial credit for the attempt would be nice :)
--

--semS (MoveTo x' y') (m, x, y) = case m of
--                                   Down -> ((m, x', y'), [(x, y, x', y')])
--                                   Up -> ((m, x', y'), 
--                                         [(x', y', x', y')])

semS (MoveTo x' y') (m, x, y) = ((m, x', y'), [(x, y, x', y')])

semS (Seq c c') (m, x, y) = 
	(fst (semS c' (m, x, y)) , 
	 -- [(x,y,x,y)]++(snd (semS c' (m, x, y)) ))
     (snd (semS c (m, x, y)))++(snd (semS c' (m, 
     stateGetX (fst (semS c (m, x, y))), 
     stateGetY (fst (semS c (m, x, y)))))))

sem' :: Cmd -> Lines
sem' c = snd (semS c (Up, 0, 0))

-- Test code: 
my_lines = 
    sem' (Seq (Pen Down) (Seq 
            (MoveTo 0 100) (Seq 
                (MoveTo 100 100) (Seq 
                    (Pen Up) (MoveTo 0 0)))))

