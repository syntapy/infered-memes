
        else if (input[i] == '!' || input[i] == '(' || input[i] == ')')
            AddToken(&head, input[i++]);

        else if (input[i] == '4' || input[i] == '3')
            AddToken(&head, input[i++]);

        else if (input[i] == '{' || input[i] == '}')
            AddToken(&head, input[i++]);

        else if (input[i] == '[' || input[i] == ']')
            AddToken(&head, input[i++]);



        else if (input[i] == 

            while (1)
            {
                if (input[i] >= LOWER_CASE_FIRST && input[i] <= LOWER_CASE_LAST)
                {

                    AddToken(&head, input[i++]);
                    while (input[i] == ',' || input[i] == ' ')
                        i++;

                    continue;
                }

                else if (input[i] >= UPPER_CASE_FIRST && input[i] <= UPPER_CASE_LAST)
                {
                    AddToken(&head, input[i++]);

                    if (input[i] == '[')
                    {
                        i++;
                        while (1)
                        {
                            if (input[i] >= LOWER_CASE_FIRST && input[i] <= LOWER_CASE_LAST)
                                AddToken(&head, input[i++]);

                            if (input[i] == ',' || input[i] == ' ')
                                i++;

                            if (input[i] == ']')
                            {   i++;
                                break;
                            }
                        }
                    }
                }

                else
                    break;
            }

            //else
            //    InconsistencyErr("PrpsTokenizer 1");
        }

        else if (input[i] >= UPPER_CASE_FIRST && input[i] <= UPPER_CASE_LAST)
        {
            AddToken(&head, input[i++]);

            if (input[i] == '[')
            {
                i++;
                if (input[i] >= LOWER_CASE_FIRST && input[i] <= LOWER_CASE_LAST)
                {
                    AddToken(&head, input[i++]);

                    if (input[i] == ']')
                    { 	i++;
                        continue;
                    }
                }
            }
        }

        else if (input[i] == '^')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == 'v')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == '|')
        {
            i++;
            if (input[i] == '|')
            {
                AddToken(&head, input[i++]);
                continue;
            }
        }

        else if (input[i] == '=')
        {
            AddToken(&head, input[i++]);
            continue;
        }

        else if (input[i] == '-')
        {
            i++;
            if (input[i] == '>')
            {
                AddToken(&head, input[i++]);
                continue;
            }
        }

