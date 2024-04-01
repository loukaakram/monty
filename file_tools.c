#include "monty.h"

/**
 * open_file_custom - Opens a file
 * @file_path: The file path
 * Return: void
 */

void open_file_custom(char *file_path)
{
	FILE *file_descriptor = fopen(file_path, "r");

	if (file_path == NULL || file_descriptor == NULL)
		error_handler_custom(2, file_path);

	read_file_custom(file_descriptor);
	fclose(file_descriptor);
}


/**
 * read_file_custom - Reads a file
 * @file_descriptor: Pointer to file descriptor
 * Return: void
 */

void read_file_custom(FILE *file_descriptor)
{
	int line_number, format = 0;
	char *buffer = NULL;
	size_t buffer_size = 0;

	for (line_number = 1; getline(&buffer, &buffer_size, file_descriptor) != -1; line_number++)
	{
		format = parse_line_custom(buffer, line_number, format);
	}
	free(buffer);
}


/**
 * parse_line_custom - Separates each line into tokens to determine
 * which function to call
 * @line: Line from the file
 * @line_number: Line number
 * @format: Storage format. If 0 Nodes will be entered as a stack.
 * If 1 nodes will be entered as a queue.
 * Return: Returns 0 if the opcode is stack. 1 if queue.
 */

int parse_line_custom(char *line, int line_number, int format)
{
	char *opcode, *value;
	const char *delimiter = "\n ";

	if (line == NULL)
		error_handler_custom(4);

	opcode = strtok(line, delimiter);
	if (opcode == NULL)
		return (format);
	value = strtok(NULL, delimiter);

	if (strcmp(opcode, "stack") == 0)
		return (0);
	if (strcmp(opcode, "queue") == 0)
		return (1);

	find_func_custom(opcode, value, line_number, format);
	return (format);
}

/**
 * find_func_custom - Finds the appropriate function for the opcode
 * @opcode: Opcode
 * @value: Argument of opcode
 * @format: Storage format. If 0 Nodes will be entered as a stack.
 * @line_number: Line number
 * If 1 nodes will be entered as a queue.
 * Return: void
 */
void find_func_custom(char *opcode, char *value, int line_number, int format)
{
	int i;
	int flag;

	instruction_t func_list[] = {
		{"push", add_to_stack},
		{"pall", print_stack},
		{"pint", print_top},
		{"pop", pop_top},
		{"nop", nop},
		{"swap", swap_nodes},
		{"add", add_nodes},
		{"sub", sub_nodes},
		{"div", div_nodes},
		{"mul", mul_nodes},
		{"mod", mod_nodes},
		{"pchar", print_char},
		{"pstr", print_str},
		{"rotl", rotl},
		{"rotr", rotr},
		{NULL, NULL}
	};

	if (opcode[0] == '#')
		return;

	for (flag = 1, i = 0; func_list[i].opcode != NULL; i++)
	{
		if (strcmp(opcode, func_list[i].opcode) == 0)
		{
			call_func_custom(func_list[i].f, opcode, value, line_number, format);
			flag = 0;
		}
	}
	if (flag == 1)
		error_handler_custom(3, line_number, opcode);
}


/**
 * call_func_custom - Calls the required function.
 * @func: Pointer to the function that is about to be called.
 * @opcode: String representing the opcode.
 * @value: String representing a numeric value.
 * @line_number: Line number for the instruction.
 * @format: Format specifier. If 0 Nodes will be entered as a stack.
 * If 1 nodes will be entered as a queue.
 */
void call_func_custom(op_func func, char *opcode, char *value, int line_number, int format)
{
	stack_t *node;
	int flag;
	int i;

	flag = 1;
	if (strcmp(opcode, "push") == 0)
	{
		if (value != NULL && value[0] == '-')
		{
			value = value + 1;
			flag = -1;
		}
		if (value == NULL)
			error_handler_custom(5, line_number);
		for (i = 0; value[i] != '\0'; i++)
		{
			if (isdigit(value[i]) == 0)
				error_handler_custom(5, line_number);
		}
		node = create_node(atoi(value) * flag);
		if (format == 0)
			func(&node, line_number);
		if (format == 1)
			add_to_queue(&node, line_number);
	}
	else
		func(&head, line_number);
}
