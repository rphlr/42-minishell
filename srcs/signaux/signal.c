/*void	rl_replace_line(const char *text, int clear_undo)
{
	int	len;

	len = strlen(text);
	if (len >= rl_line_buffer_line)
		rl_extend_line_buffer (len);
	strcpy (rl_line_buffer, text);
	rl_end = len;
	if (clear_undo)
		rl_free_undo_list ();
	_rl_fix_point (1);
}
*/
