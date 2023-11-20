require 'faker'

1_000.times do
  first_name = Faker::Name.first_name
  last_name = Faker::Name.last_name
  birth_year = Faker::Date.between(from: '1950-01-01', to: '2003-12-31').year
  emp_year = Faker::Date.between(from: '2015-01-01', to: '2022-12-31').year

  print "#{last_name},#{first_name},#{birth_year},#{emp_year}\n"
end


