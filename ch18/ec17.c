#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

struct Address {
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address **rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};

void Database_close (struct Connection *conn); // Forward declaration

void die (struct Connection *conn, const char *message)
{
	if (errno) {
		perror(message);
	} else {
		printf("ERROR: %s\n", message);
	}
	
	Database_close(conn);
	exit(1);
}

void Address_print(struct Address *addr)
{
	printf("%d %s %s\n", addr->id, addr->name, addr->email);
}

void Database_load(struct Connection *conn)
{
	// The db file is structured so that it contains this data 
	// in this order:
	//
	// (int)max_data : 4 bytes
	// (int)max_rows : 4 bytes
	// (struct Address **)rows: The remainder of the file
	//
	// So we then read our blocks of data into the proper variables.
	
	int rc = fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	if (rc != 1) die(conn, "Failed to read max data size.");
	
	rc = fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	if (rc != 1) die(conn, "Failed to read max row size.");
	
	conn->db->rows = (struct Address **)malloc(sizeof(struct Address *) * conn->db->max_rows);
	if (!conn->db->max_rows) die(conn, "Could not allocate space for rows.");
	
	// Now load the addresses one by one
	size_t i = 0;
	for (i=0; i < conn->db->max_rows; i++) {
		
		// Allocate enough space for our row
		conn->db->rows[i] = (struct Address *)malloc(sizeof(struct Address));
		// Create an alias for our row to prevent carpal tunnel
		struct Address *row = conn->db->rows[i];
		
		// Addresses, currently, are this data in this order:
		//
		// (int)id : 4 bytes
		// (int)set : 4 bytes
		// (char *)name : (max_data * sizeof(char)) bytes
		// (char *)email : (max_data * sizeof(char)) bytes
		
		rc = fread(&row->id, sizeof(row->id), 1, conn->file);
		if (rc != 1) die(conn, "Could not load address->id.");
		
		rc = fread(&row->set, sizeof(row->set), 1, conn->file);
		if (rc != 1) die(conn, "Could not load address->set.");
		
		// Have to allocate memory for fields
		row->name = malloc(conn->db->max_data * sizeof(*row->name));
		row->email = malloc(conn->db->max_data * sizeof(*row->email));
		
		// Now we load the values
		rc = fread(row->name, conn->db->max_data * sizeof(*row->name), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load name.");
		
		rc = fread(row->email, conn->db->max_data * sizeof(*row->email), 1, conn->file);
		if (rc != 1) die(conn, "Failed to load email.");
	}
	
	
}

struct Connection *Database_open (const char *filename, char mode)
{
	struct Connection *conn = malloc(sizeof(struct Connection));
	if (!conn) die(NULL, "Memory error.");
	
	conn->db = malloc(sizeof(struct Database));
	if (!conn->db) die(conn, "Memory error.");
	
	if (mode=='c') {
		conn->file = fopen(filename, "w");
	} else {
		conn->file = fopen(filename, "r+");
		
		if (conn->file) {
			Database_load(conn);
		}
	}
	
	if (!conn->file) die(conn, "Failed to open file.");
	
	return conn;
}

void Database_close (struct Connection *conn)
{
	// Make sure to free child blocks before parent blocks
	if (conn) {
		if (conn->file) fclose(conn->file);
		if (conn->db) {
			if (conn->db->rows) {
				for (size_t i=0; i < conn->db->max_rows; i++) {
					free(conn->db->rows[i]->name);
					free(conn->db->rows[i]->email);
					free(conn->db->rows[i]);
				}
				free(conn->db->rows);
			}
			free(conn->db);
		}
		free(conn);
	}
}

void Database_write (struct Connection *conn)
{
	rewind(conn->file);
	// write max_data and max_rows first, so they can be read first
	int rc = fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write max data size.");
	
	rc = fwrite(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	if (rc != 1) die(conn, "Failed to write max rows.");

	// Now we have to write each record.
	for (size_t i = 0; i < conn->db->max_rows; i++) {
		struct Address *row = conn->db->rows[i];
		rc = fwrite(&row->id, sizeof(row->id), 1, conn->file);
		if (rc != 1) die(conn, "Failed to write row id.");
		
		rc = fwrite(&row->set, sizeof(row->set), 1, conn->file);
		if (rc != 1) die(conn, "Failed to write row set flag.");
		
		rc = fwrite(row->name, sizeof(char) * conn->db->max_data, 1, conn->file);
		if (rc != 1) die(conn, "Failed to write name.");
		
		rc = fwrite(row->email, sizeof(char) * conn->db->max_data, 1, conn->file);
		if (rc != 1) die(conn, "Failed to write email.");
	}
	
	if (fflush(conn->file) == -1) die(conn, "Cannot flush database.");
}

void Database_create (struct Connection *conn, int max_data, int max_rows)
{
	int i = 0;
	
	conn->db->max_data = max_data;
	conn->db->max_rows = max_rows;
	
	// Allocate space for rows.
	conn->db->rows = (struct Address**)malloc(sizeof(struct Address *) * max_rows);
	
	for(i=0; i < max_rows; i++) {
		// Allocate memory for entry
		conn->db->rows[i] = (struct Address *)malloc(sizeof(struct Address));
		conn->db->rows[i]->name = (char *)malloc(conn->db->max_data);
		conn->db->rows[i]->email = (char *)malloc(conn->db->max_data);
		// Fill in the default (blank) information
		conn->db->rows[i]->id = i;
		conn->db->rows[i]->set = 0;
		conn->db->rows[i]->name = (char *)memset(conn->db->rows[i]->name, ' ', conn->db->max_data);
		conn->db->rows[i]->email = (char *)memset(conn->db->rows[i]->email, ' ', conn->db->max_data);
	}
}

void Database_set (struct Connection *conn, int id, const char *name, const char *email)
{
	struct Address *addr = conn->db->rows[id];
	if (addr->set) die(conn, "Already set, delete it first.");
	
	addr->set = 1;

	// Preserving this mistake here so I don't repeat it.
	// There is no reason to allocate this memory, since it is allocated
	// on load. Doing this orphans the originally allocated block,
	// causing a leak.

	// addr->name = malloc(sizeof(char) * conn->db->max_data);
	// addr->email = malloc(sizeof(char) * conn->db->max_data);

	char *res = strncpy(addr->name, name, conn->db->max_data);
	// Since strncpy doesn't automatically null-terminate:
	addr->name[conn->db->max_data - 1] = '\0';
	if (!res) die(conn, "Name copy failed.");
	
	res = strncpy(addr->email, email, conn->db->max_data);
	addr->email[conn->db->max_data - 1] = '\0';
	if (!res) die(conn, "Email copy failed.");

}

void Database_get (struct Connection *conn, int id)
{
	struct Address *addr = conn->db->rows[id];
	
	if (addr->set) {
		Address_print(addr);
	} else {
		die(conn, "ID is not set.");
	}
}

void Database_delete (struct Connection *conn, int id)
{
	// This is essentially a duplicate of our code in Database_create
	// but just for one record. It may be cleaner to refactor this as a
	// separate function, or to just call this within the create function.
	struct Address *addr = (struct Address *)malloc(sizeof(struct Address));
	addr->id = id;
	addr->set = 0;
	addr->name = (char *)malloc(conn->db->max_data);
	addr->name = (char *)memset(addr->name, ' ', conn->db->max_data);
	addr->email = (char *)malloc(conn->db->max_data);
	addr->email = (char *)memset(addr->name, ' ', conn->db->max_data);
	conn->db->rows[id] = addr;
}

void Database_list (struct Connection *conn)
{
	int i = 0;
	struct Database *db = conn->db;
	
	for (i=0; i < db->max_rows; i++) {
		struct Address *cur = db->rows[i];
		
		if (cur->set) {
			Address_print(cur);
		}
	}
}

int main (int argc, char *argv[])
{
	if (argc < 3) die(NULL, "USAGE: ./ex17 <dbfile> <action> [action params]");
	
	char *filename = argv[1];
	char action = argv[2][0];
	struct Connection *conn = Database_open(filename, action);
	int id = 0;
	
	if (argc > 3) id = atoi(argv[3]);
	
	switch (action) {
		case 'c':
			if (argc != 5) die(conn, "Need to specify maximum data length and maximum number of rows.");

			Database_create(conn, id, atoi(argv[4]));
			Database_write(conn);
			break;
		
		case 'g':
			if (argc != 4) die(conn, "Need an ID to retrieve.");
			
			Database_get(conn, id);
			break;

		case 's':
			if (argc != 6) die(conn, "Need ID, name, and email to set.");
			
			Database_set(conn, id, argv[4], argv[5]);
			Database_write(conn);
			break;

		case 'd':
			if (argc != 4) die(conn, "Need ID to delete.");
			
			Database_delete(conn, id);
			Database_write(conn);
			break;
		
		case 'l':
			Database_list(conn);
			break;

		default:
			die(conn, "Invalid action, only: c=create, g=get, s=set, d=delete, l=list");
	}
	
	Database_close(conn);
	return 0;
}
