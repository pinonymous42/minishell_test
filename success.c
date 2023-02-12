/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   success.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: matsushimakouhei <matsushimakouhei@stud    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 00:04:47 by matsushimak       #+#    #+#             */
/*   Updated: 2023/02/12 00:41:11 by matsushimak      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define STDIN 0
#define STDOUT 1

typedef struct s_info{
    int     input_fd;
    int     output_fd;
    int     argc;
    char    ***cmd;
    char    **path;
    char    **envp;
    int     heredoc_flag;
}t_info;

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*ret;
	size_t	tmp;

	tmp = len;
	if (s == NULL)
		return (NULL);
	if (strlen(s) < start)
		return (calloc(1, 1));
	if (strlen(s + start) < len)
		tmp = strlen(s + start);
	ret = (char *)malloc(sizeof(char) * (tmp + 1));
	if (!ret)
		return (0);
	strlcpy(ret, s + start, tmp + 1);
	return (ret);
}

static int	count_sep(char const *s, char c)
{
	int	amount;
	int	count;

	amount = 0;
	count = 0;
	while (*s)
	{
		if (*s == c)
		{
			if (amount != 0)
				count++;
			amount = 0;
		}
		else
			amount++;
		s++;
	}
	if (amount != 0)
		return (count + 1);
	return (count);
}

static int	check(char **ans, size_t i)
{
	if (ans[i] == NULL)
	{
		while (i >= 0)
		{
			free(ans[i]);
			i--;
		}
		free(ans);
		return (0);
	}
	return (1);
}

static char	**do_it(const char *s, char c, char **ans, size_t i)
{
	size_t	len;

	while (*s)
	{
		if (*s == c)
			s++;
		else
		{
			len = 0;
			while (*s && *s != c)
			{
				len++;
				s++;
			}
			ans[i] = ft_substr(s - len, 0, len);
			if (!check(ans, i))
				return (NULL);
			i++;
		}
	}
	ans[i] = NULL;
	return (ans);
}

char	**ft_split(char const *s, char c)
{
	char	**ans;

	if (s == NULL)
		return (NULL);
	ans = (char **)malloc(sizeof(char *) * (count_sep(s, c) + 1));
	if (!ans)
		return (NULL);
	ans = do_it(s, c, ans, 0);
	return (ans);
}

// #include <stdio.h>
// int main(void)
// {
// 	char **tab = ft_split("ab\0cd", '\0');
// 	printf("%s\n", tab[0]);
// 	return (0);
// }


char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ret;

	if (s1 == NULL || s2 == NULL)
		return (NULL);
	ret = (char *)malloc(sizeof(char) * (strlen(s1) + strlen(s2) + 1));
	if (!ret)
		return (NULL);
	*ret = '\0';
	strlcat(ret, s1, strlen(s1) + 1);
	strlcat(ret, s2, strlen(s1) + strlen(s2) + 1);
	return (ret);
}
// char *cmd1[] = {"test.c", NULL};
// char *cmd2[] = {"cat", NULL};
// char *cmd3[] = {"cat", NULL};
// char *cmd4[] = {"output.txt", NULL};
// char **cmds[] = {cmd1, cmd2, cmd3, cmd4};
// int cmd_n = 4;

char    *make_exepath(char *path, char *command)
{
    char    *exe_path;
    char    *tmp;

    tmp = ft_strjoin(path, "/");
    exe_path = ft_strjoin(tmp, command);
    free(tmp);
    return (exe_path);
}

void    dopipes(int i, t_info *info)
{
    pid_t   ret;
    int     pipefd[2];
    //[0]: read
    //[1]: write
    char    *exe_path;
    int     index;

    exe_path = NULL;
    index = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    if (info->argc - 2 == i)//一番左のコマンドを実行
    {
        dup2(info->input_fd, STDIN);
        // exe_path = NULL;
        // while (access(exe_path, X_OK))
        // {
        //     // printf("%s, %d\n", __FILE__, __LINE__);
        //     exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
        //     // fprintf(stdout, "%s\n", exe_path);
        //     index++;
        // }
        execve("/bin/cat", (info->cmd)[info->argc - i - 1], info->envp);
		// execvp("cat", (info->cmd)[info->argc - i - 1]);
    }
    else if (i == 0)//一番右のコマンドを実行
    {
        dup2(info->output_fd, STDOUT);
        dopipes(i + 1, info);
    }
    else//真ん中ら辺のコマンド
    {
        pipe(pipefd);
        if ((ret = fork()) == -1)
        {
            perror("fork");
            exit(1);
        }
        if (ret == 0)
        {
            close(pipefd[0]);
            dup2(pipefd[1], STDOUT);
            close(pipefd[1]);
            dopipes(i + 1, info);
        }
        else
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN);
            close(pipefd[0]);
            // exe_path = NULL;
            // while (access(exe_path, X_OK))
            // {
            //     exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
            //     // printf("%s\n", exe_path);
            //     index++;
            //     // printf("%d\n", access(exe_path, X_OK));
            // }
            execve("/bin/cat", (info->cmd)[info->argc - i - 1], info->envp);
			// execvp("cat", (info->cmd)[info->argc - i - 1]);
        }
    }
}

char    **make_path_list(char **envp)
{
    char    *pointer_path;

    while (strncmp(*envp, "PATH", 4))
        envp++;
    pointer_path = strchr(*envp, '=');
    return (ft_split(pointer_path + 1, ':'));
}

void    multiple_pipes(t_info *info)
{
    pid_t   ret;
    // info->path = make_path_list(info->envp);
    // while (*(info->path))
    //     printf("%s\n", *(info->path)++);
    // printf("%s, %d\n", __FILE__, __LINE__);
    if ((ret = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }
    else if (ret == 0)
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        dopipes(0, info);
    }
    else
        wait(NULL);
}

int main(int argc, char *argv[], char *envp[])
{
    t_info  info;
    pid_t ret;
    int     i;
    
    if (argc < 5)
    {
        perror("argument number");
        exit(1);
    }
    //   int input_fd = open(cmds[0][0], O_RDONLY);
    // int output_fd = open(cmds[cmd_n - 1][0], (O_WRONLY | O_CREAT), 0777);
    // int input_fd = open(argv[1], O_RDONLY);
    // int output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT), 0777);
    info.input_fd = open(argv[1], O_RDONLY);
    info.output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT), 0777);
    info.argc = argc - 1;
    info.envp = envp;
    if (!strncmp(argv[1], "here_doc", 8))
        ;
    else
    {
        i = 1;
        info.cmd = (char ***)malloc(sizeof(char **) * argc);
        // printf("%s, %d\n", __FILE__, __LINE__);
        while (argv[i])
        {
            info.cmd[i - 1] = ft_split(argv[i], ' ');
            i++;
        }
        // printf("%s, %d\n", __FILE__, __LINE__);
        info.cmd[i] = NULL;
        
        int j;
        i = 0;
        while (info.cmd[i])
        {
            j = 0;
            printf("-----------------------------\n");
            while (info.cmd[i][j])
            {
                // printf("i: %d, j: %d\n", i, j);
                printf("%s\n", info.cmd[i][j]);
                j += 1;
            }
        printf("%s\n", info.cmd[i][j]);
            i += 1;
        }
        printf("%d\n", info.argc);
        multiple_pipes(&info);
    }
    close(info.input_fd);
    close(info.output_fd);
    return (0);
}
