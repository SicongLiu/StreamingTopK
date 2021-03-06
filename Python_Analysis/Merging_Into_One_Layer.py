
import numpy as np
dimension = 4
cardinality = 200000
# cardi = '200k'
layer_count = 25
# data_distribution = ['anti_correlated', 'correlated', 'random']
layer_file_folders = '/Users/sicongliu/Desktop/StreamingTopK/H2_ALSH/qhull_data/Synthetic/'
save_file_folders1 = 'anti_correlated_' + str(dimension) + '_' + str(cardinality) + '.txt'
save_file_folders2 = 'correlated_' + str(dimension) + '_' + str(cardinality) + '.txt'
save_file_folders3 = 'random_' + str(dimension) + '_' + str(cardinality) + '.txt'

f1 = open(save_file_folders1, "w")
f2 = open(save_file_folders2, "w")
f3 = open(save_file_folders3, "w")
total_data_count1 = 0
total_data_count2 = 0
total_data_count3 = 0

total_data1 = []
total_data2 = []
total_data3 = []
for i in range(layer_count):
    layer_file11 = layer_file_folders + 'anti_correlated_' + str(dimension) + '_' + str(cardinality) + \
                 '_qhull_layer_'+ str(i)

    layer_file22 = layer_file_folders + 'correlated_' + str(dimension) + '_' + str(cardinality) + \
                   '_qhull_layer_' + str(i)

    layer_file33 = layer_file_folders + 'random_' + str(dimension) + '_' + str(cardinality) + \
                   '_qhull_layer_' + str(i)

    f11 = open(layer_file11, 'r')
    f11_lines = f11.readlines()
    for line in f11_lines[2:]:
        myarray = np.fromstring(line, dtype=float, sep=' ')
        total_data1.append(myarray)
    total_data_count1 = total_data_count1 + int(f11_lines[1])
    f11.close()

    f22 = open(layer_file22, 'r')
    f22_lines = f22.readlines()
    for line in f22_lines[2:]:
        myarray = np.fromstring(line, dtype=float, sep=' ')
        total_data2.append(myarray)
    total_data_count2 = total_data_count2 + int(f22_lines[1])
    f22.close()

    f33 = open(layer_file33, 'r')
    f33_lines = f33.readlines()
    for line in f33_lines[2:]:
        myarray = np.fromstring(line, dtype=float, sep=' ')
        total_data3.append(myarray)
    total_data_count3 = total_data_count3 + int(f33_lines[1])
    f33.close()
# End-For
# save data to file
temp_data = []
temp_data.append(dimension)
temp_data.append(total_data_count1)
temp_data = np.asarray(temp_data)
np.savetxt(save_file_folders1, temp_data, delimiter=',', fmt='%i')
f1.close()

f1 = open(save_file_folders1, 'ab')
np.savetxt(f1, total_data1, fmt='%10.6f')
f1.close()

temp_data = []
temp_data.append(dimension)
temp_data.append(total_data_count2)
temp_data = np.asarray(temp_data)
np.savetxt(save_file_folders2, temp_data, delimiter=',', fmt='%i')
f2.close()

f2 = open(save_file_folders2, 'ab')
np.savetxt(f2, total_data2, fmt='%10.6f')
f2.close()

temp_data = []
temp_data.append(dimension)
temp_data.append(total_data_count3)
temp_data = np.asarray(temp_data)
np.savetxt(save_file_folders3, temp_data, delimiter=',', fmt='%i')
f3.close()

f3 = open(save_file_folders3, 'ab')
np.savetxt(f3, total_data3, fmt='%10.6f')
f3.close()
f3.close()

