# frozen_string_literal: true

require 'csv'

app_time_max_regex = /Max\s+(\d+\.\d+)/
app_time_mean_regex = /Mean\s+(\d+\.\d+)/
app_time_min_regex = /Min\s+(\d+\.\d+)/
app_time_stddev_regex = /Stddev\s+(\d+\.\d+)/
app_time_aggregate_regex = /Aggregate\s+(\d+\.\d+)/
mpi_time_max_regex = /Max.*\s+(\d+\.\d+)\s+\d+/
mpi_time_mean_regex = /Mean.*\s+(\d+\.\d+)/
mpi_time_min_regex = /Min.*\s+(\d+\.\d+)\s+\d+/
mpi_time_stddev_regex = /Stddev.*\s+(\d+\.\d+)/
mpi_time_aggregate_regex = /Aggregate.*\s+(\d+\.\d+)\s+\d+\.\d+/
aggregate_percentage_regex = /Aggregate\s+\d+\.\d+\s+\d+\.\d+\s+(\d+\.\d+)/

[1, 10].each do |records|
  root_dir = "./logs/#{records}k-records"

  # Prepare the CSV file
  CSV.open("./logs/output#{records}.csv", 'wb') do |csv|
    # Write the header row
    csv << ['Directory', 'AppTime Max', 'AppTime Mean', 'AppTime Min', 'AppTime Stddev', 'AppTime Aggregate',
            'MPITime Max', 'MPITime Mean', 'MPITime Min', 'MPITime Stddev', 'MPITime Aggregate', 'MPI%']

    # Iterate through each subdirectory
    Dir.foreach(root_dir) do |subdir|
      next if (subdir == '.') || (subdir == '..')

      # Path to the subdirectory
      subdir_path = File.join(root_dir, subdir)

      # Iterate through each file in the subdirectory
      Dir.foreach(subdir_path) do |file|
        next unless file.end_with?('.mpiP')

        # Path to the file
        file_path = File.join(subdir_path, file)

        # Read and process the file
        File.open(file_path) do |f|
          contents = f.read

          # Extracting AppTime statistics
          app_time_max = contents.match(app_time_max_regex)[1].to_f
          app_time_mean = contents.match(app_time_mean_regex)[1].to_f
          app_time_min = contents.match(app_time_min_regex)[1].to_f
          app_time_stddev = contents.match(app_time_stddev_regex)[1].to_f
          app_time_aggregate = contents.match(app_time_aggregate_regex)[1].to_f

          # Extracting MPITime statistics
          mpi_time_max = contents.match(mpi_time_max_regex)[1].to_f
          mpi_time_mean = contents.match(mpi_time_mean_regex)[1].to_f
          mpi_time_min = contents.match(mpi_time_min_regex)[1].to_f
          mpi_time_stddev = contents.match(mpi_time_stddev_regex)[1].to_f
          mpi_time_aggregate = contents.match(mpi_time_aggregate_regex)[1].to_f
          mpi_time_aggregate_percentage = contents.match(aggregate_percentage_regex)[1].to_f

          # Write the data to the CSV
          csv << [subdir.to_s[1], app_time_max, app_time_mean, app_time_min, app_time_stddev, app_time_aggregate,
                  mpi_time_max, mpi_time_mean, mpi_time_min, mpi_time_stddev, mpi_time_aggregate, mpi_time_aggregate_percentage]
        end
      end
    end
  end
end
