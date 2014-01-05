class Setting
	include ActiveModel::Validations
  	include ActiveModel::Conversion
  	extend ActiveModel::Naming

	validates :color, presence: true
 
  	attr_accessor :color

	def initialize(attributes = {})
		attributes.each do |name, value|
			send("#{name}=", value)
		end
	end

	def persisted?
		false
	end
end