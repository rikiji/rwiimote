#!/usr/bin/ruby
require 'rubygems'
require 'rwiimote'

w=Wiimote.new
puts "press both 1 and 2 keys to connect"
puts "remember to set your own mac address"
exit unless w.connect "00:11:22:33:44:55"
puts "connected!"

w.acc= true
w.led[3].on

puts "shake to disconnect!!"
while w.open?
  w.disconnect unless w.update
	
  puts "force x: " + w.force_x.to_s
  puts "force y: " + w.force_y.to_s
  puts "force z: " + w.force_z.to_s

  w.disconnect if w.force > 2
end
puts "well done!"


