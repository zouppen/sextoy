{-# LANGUAGE ForeignFunctionInterface, EmptyDataDecls #-}
-- |Small library for providing minimal functionality to support
-- controlling the toy vibrator.
module Ftdi (FtdiHandle,new,free,onoff,function) where

import Control.Applicative
import Data.Word
import Foreign.C
import Foreign.Ptr

data FtdiContext
type FtdiHandle = Ptr FtdiContext

-- Own implementation
foreign import ccall ftdi_context_new  :: IO FtdiHandle
foreign import ccall ftdi_bitbang_init :: FtdiHandle -> CString -> IO CInt
foreign import ccall ftdi_pulldown     :: FtdiHandle -> Word8 -> IO CInt

-- Libftdi provided functions

-- |Frees handle to the toy vibrator device allocated by `new`.
foreign import ccall "ftdi_free" free      :: FtdiHandle -> IO ()
foreign import ccall ftdi_get_error_string :: FtdiHandle -> IO CString

-- |Opens toy vibrator device with given serial number and puts it in
-- bit-banging mode. Serial number may be obtained with `usb-devices`
-- command. An example of serial number: `FTGDL1AX`.
new :: String -> IO FtdiHandle
new serial = do
  h <- ftdi_context_new
  ret <- withCString serial $ ftdi_bitbang_init h
  case ret of
    0 -> return h
    _ -> do
      desc <- ftdi_get_error_string h >>= peekCString
      free h
      fail $
        "Unable to initialize the sex toy: " ++ desc ++
        " (libftdi error " ++ show ret ++ ")"

-- |Runs given action and throw an error if needed.
failFilter :: FtdiHandle -> IO CInt -> IO ()
failFilter h act = do
  code <- act
  case code of
    0  -> pure ()
    _  -> do 
      desc <- ftdi_get_error_string h >>= peekCString
      fail $
        "Unable to communicate with the sex toy: " ++ desc ++
        " (libftdi error " ++ show code ++ ")"

-- |Pushes ON/OFF button.
onoff :: FtdiHandle -> IO ()
onoff h = failFilter h $ ftdi_pulldown h 0x01 -- Orange wire

-- |Pushes FUNCTION button.
function :: FtdiHandle -> IO ()
function h = failFilter h $ ftdi_pulldown h 0x02 -- Yellow wire
