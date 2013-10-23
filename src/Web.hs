{-# LANGUAGE OverloadedStrings #-}
module Main where 

import Control.Monad.IO.Class
import Control.Monad (replicateM_)
import Data.ByteString.Lazy.Char8 (ByteString)
import Data.Text (Text,unpack)
import Data.Text.Read (decimal)
import Network.HTTP.Types (ok200,badRequest400)
import Network.Wai
import Network.Wai.Handler.Warp (run)

import Ftdi

main = do
  let device = "FTFRXISS"
  let port = 3000
  putStrLn $ "Connecting " ++ show device ++ " and listening to port " ++ show port
  h <- new device
  run port $ app h

app :: FtdiHandle -> Application
app h req = case (requestMethod req,pathInfo req) of
  ("POST",["onoff"]) -> do
    liftIO $ onoff h
    good "OK\n"
  ("POST",["onoff",skips]) -> do
    case validateSkips skips of
      Just s -> do
        liftIO $ onoff h
        liftIO $ replicateM_ s $ function h
        good "OK\n"
      Nothing -> bad "Mode must be a integer between 0 and 37, inclusive\n"
  ("POST",["function"]) -> do
    liftIO $ function h
    good "OK\n"
  _ -> bad "Unknown command"

bad,good :: Monad m => ByteString -> m Response
bad  = textualResponse badRequest400 
good = textualResponse ok200

textualResponse code text = return $
                            responseLBS code
                            [("Content-Type", "text/plain")]
                            text

validateSkips :: Integral a => Text -> Maybe a
validateSkips t = case decimal t of
  Right (a,"") -> if a<38
                  then Just a 
                  else Nothing
  _ -> Nothing
