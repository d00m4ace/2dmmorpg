//password rules check 
bool is_valid_password(const char* password)
{
	int password_len = strlen(password);

	if(password_len < 6 || password_len > 20)
		return false;

	for(int i = 0; i < password_len; i++)
	{
		if(!((password[i] >= 'a' && password[i] <= 'z') || (password[i] >= 'A' && password[i] <= 'Z') || (password[i] >= '0' && password[i] <= '9')))
			return false;
	}

	return true;
}

bool is_valid_username(const char* username)
{
	int username_len = strlen(username);

	if(username_len < 3 || username_len > 10)
		return false;

	if(!(username[0] >= 'A' && username[0] <= 'Z'))
		return false;

	for(int i = 1; i < username_len; i++)
	{
		if(!((username[i] >= 'a' && username[i] <= 'z') || (username[i] >= '0' && username[i] <= '9')))
			return false;
	}

	return true;
}

bool is_valid_email(const char* email)
{
	int email_len = strlen(email);

	if(email_len < 6 || email_len > 50)
		return false;

	int at_count = 0;

	for(int i = 0; i < email_len; i++)
	{
		if(!((email[i] >= 'a' && email[i] <= 'z') || (email[i] >= 'A' && email[i] <= 'Z') || (email[i] >= '0' && email[i] <= '9') || email[i] == '@' || email[i] == '.' || email[i] == '_' || email[i] == '-'))
			return false;

		if(email[i] == '@')
		{
			if(i == 0 || i == email_len - 1)
				return false;
			at_count++;
		}
	}

	if(at_count != 1)
		return false;

	return true;
}
