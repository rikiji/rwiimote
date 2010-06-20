require 'rubygems'
require 'rake'

spec = Gem::Specification.new do |s|
  s.name = "rwiimote"
  s.version = "0.0.1"
  s.summary = "Control and get data from your wiimote with ruby."
  s.description = "Control and get data from your wiimote with ruby. Wrapper around libcwiimote."
  s.licenses = ['GPL']

  s.author = "Riccardo Cecolin"
  s.email = "rikiji@playkanji.com"
  s.homepage = "http://www.rikiji.it"

  s.files = FileList["examples/*.rb","ext/rwiimote/*.rb","ext/rwiimote/*.c","lib/*","LICENSE","README"]
  s.extensions = FileList["ext/rwiimote/extconf.rb"]
end

