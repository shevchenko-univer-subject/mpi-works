require 'csv'

def generate_matrix(size, max_value = 100)
  Array.new(size) { Array.new(size) { rand(max_value) } }
end

filepath = "matrices/#{ARGV[0]}.csv"
size = ARGV[1].to_i

matrix = generate_matrix(size)

CSV.open(filepath, "wb") do |csv|
  matrix.each do |row|
    csv << row
  end
end

puts "Matrix CSV file (#{filepath}) generated successfully."

