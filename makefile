client: mirror_client.o functions.o lista.o
	gcc -o client functions.o mirror_client.o lista.o

mirror_client.o:mirror_client.c
	gcc -c mirror_client.c

functions.o:functions.c
	gcc -c functions.c

lista.o:lista.c
	gcc -c lista.c

clean:
	rm -f client mirror_client.o functions.o lista.o

run1:
	client -n 1 -c common_dir -i input_dir1 -m mirror_dir1 -b 1000 -l lof_file1

run2:
	client -n 2 -c common_dir -i input_dir2 -m mirror_dir2 -b 1000 -l lof_file2

run3:
	client -n 3 -c common_dir -i input_dir3 -m mirror_dir3 -b 1000 -l lof_file3

run4:
	client -n 4 -c common_dir -i input_dir4 -m mirror_dir4 -b 1000 -l lof_file4

run5:
	client -n 5 -c common_dir -i input_dir5 -m mirror_dir5 -b 1000 -l lof_file5
