#!/usr/bin/lua
-- Test Lua program to control an external LED that is attached to GPIO21

local LED21_PATH = "/sys/class/gpio/gpio21/"  -- gpio21 sysfs path
local SYSFS_DIR = "/sys/class/gpio/"        -- gpio sysfs path
local LED_NUMBER = "21"                      -- The GPIO used

-- Example function to write a value to the GPIO
function writeGPIO(directory, filename, value)
-- Debug: print(string.format("Opening the file: %s", directory..filename))
   file = io.open(directory..filename, "w")
   file:write(value)
   file:close()
end

print("Starting the Lua LED Program")
if arg[1]==nil then
   print("This program requires a command")
   print("usage is: lua led.lua or (./led.lua) command")
   print("where command is: setup, on, off, status, close")
   do return end
end
if arg[1]=="on" then
   print("Turning the LED on")
   writeGPIO(LED21_PATH, "value", "1")
elseif arg[1]=="off" then
   print("Turning the LED off")
   writeGPIO(LED21_PATH, "value", "0")
elseif arg[1]=="setup" then
   print("Setting up the LED GPIO")
   writeGPIO(SYSFS_DIR, "export", LED_NUMBER)
   os.execute("sleep 0.1")   -- to ensure gpio exported by Linux
   writeGPIO(LED21_PATH, "direction", "out")
elseif arg[1]=="close" then
   print("Closing down the LED GPIO")
   writeGPIO(SYSFS_DIR, "unexport", LED_NUMBER)
elseif arg[1]=="status" then
   print("Getting the LED status")
   file = io.open(LED21_PATH.."value", "r")
   print(string.format("The LED state is %s.", file:read()))
   file:close()
else
   print("Invalid command!")
end
print("End of the Lua LED Program")

