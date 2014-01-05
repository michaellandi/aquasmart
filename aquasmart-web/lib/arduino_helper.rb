class ArduinoStatus
	def initialize(color, temp)
		@color = color
		@temp = temp
	end

	def color
		@color
	end

	def temp
		@temp
	end
end

class ArduinoHelper
	$serial_baud = 9600
	$serial_port = '/dev/tty.usbmodem1411'

	@@defined_colors = [ 'Black', 'Blue', 'White', 'Purple', 'Red', 'Green' ]

	def initialize
		@sp = SerialPort.new $serial_port, $serial_baud, 8, 1, SerialPort::NONE
	end

    def status
        @sp.write 's'

        begin
        	status = @sp.readline.strip.split('|')
        	color = Integer(status.first)
        	temp = Float(status.last)

        	return ArduinoStatus.new(color, temp)
    	rescue
    		return ArduinoStatus.new(0, 0.0)
    	end
    end

	def color=(value)
		@sp.write value
	end

	def self.colors
		return @@defined_colors
	end

	def self.controller
		if @controller.nil?
			@controller = ArduinoHelper.new
		end

		return @controller
	end
end