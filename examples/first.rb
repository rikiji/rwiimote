#!/usr/bin/ruby
require 'rubygems'
require 'rwiimote'

w=Wiimote.new
puts "press both 1 and 2 keys to connect"
puts "remember to set your own mac address"
exit unless w.connect "00:11:22:33:44:55"
puts "connected!"

w.led[1].on
w.led[2].on

puts "click home to disconnect"
while w.open?
  w.disconnect unless w.update
  
  puts "a!" if w.a?
  w.disconnect if w.home?
end


