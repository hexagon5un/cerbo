module Ssah.Utils where

import Data.Time
import System.Locale (defaultTimeLocale)



stripChars :: String -> String -> String
stripChars = filter . flip notElem

asFloat :: String -> Float
asFloat v =  read clean :: Float   where clean = stripChars "\"%\n+" v

dateString = do
  let now = getCurrentTime
  dstamp <- fmap (formatTime defaultTimeLocale "%Y-%m-%d") now
  return dstamp

timeString = do
  let now = getCurrentTime
  tstamp <- fmap (formatTime defaultTimeLocale "%H:%M:%S") now
  return tstamp
    
  
