require 'arduino_helper'

class HomeController < ApplicationController
	def index
		status = ArduinoHelper.controller.status;

		@colors = Hash[]
		for i in 0..ArduinoHelper.colors.count - 1
			@colors[ArduinoHelper.colors[i]] = i
		end

		@temp = status.temp
		@setting = Setting.new 
		@setting.color = status.color

		expires_now
	end

	def update
		@setting = Setting.new(params[:setting])
		if ArduinoHelper.controller.status.color != @setting.color
			ArduinoHelper.controller.color = @setting.color
		end

		sleep 4

		redirect_to action: 'success'
	end

	def success
		puts ArduinoHelper.controller.status.color
	end
end

