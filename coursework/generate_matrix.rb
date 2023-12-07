require 'csv'

def generate_matrix(size, max_value = 100)
  Array.new(size) { rand(max_value) }
end

filepath = "matrices/#{ARGV[0]}.csv"
size = ARGV[1].to_i

matrix = generate_matrix(size)

File.open(filepath, "w") do |file|
  size.times do
    file << generate_matrix(size).join(' ')
    file.write "\n"
  end
end

puts "Matrix CSV file (#{filepath}) generated successfully."

