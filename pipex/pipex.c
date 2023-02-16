/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kohmatsu <kohmatsu@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/14 15:25:23 by kohmatsu          #+#    #+#             */
/*   Updated: 2023/02/15 20:44:03 by kohmatsu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/pipex.h"
#include "./includes/step_9_10.h"
// void end(void)__attribute__((destructor));
// void end(void)
// {
//     system("leaks -q pipex");
// }

void    safty_free(char **str)
{
    int i;

    i = 0;
    while (str[i])
    {
        free(str[i]);
        i++;
    }
    free(str);
    str = NULL;
}


// void    dopipes(int i, t_info *info)
// {
//     pid_t   ret;
//     int     pipefd[2];
//     //[0]: read
//     //[1]: write
//     char    *exe_path;
//     int     index;

//     exe_path = NULL;
//     index = 0;
//     if (info->argc - 2 == i)//一番左のコマンドを実行
//     {
//         dup2(info->input_fd, STDIN);
//         while (access(exe_path, X_OK))
//         {
//             exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
//             index++;
//         }
//         execve(exe_path, (info->cmd)[info->argc - i - 1], info->envp);
//     }
//     else if (i == 0)//一番右のコマンドを実行
//     {
//         dup2(info->output_fd, STDOUT);
//         dopipes(i + 1, info);
//     }
//     else//真ん中ら辺のコマンド
//     {
//         pipe(pipefd);
//         if ((ret = fork()) == -1)
//         {
//             perror("fork");
//             exit(1);
//         }
//         if (ret == 0)
//         {
//             close(pipefd[0]);
//             dup2(pipefd[1], STDOUT);
//             close(pipefd[1]);
//             dopipes(i + 1, info);
//         }
//         else
//         {
//             close(pipefd[1]);
//             dup2(pipefd[0], STDIN);
//             close(pipefd[0]);
//             while (access(exe_path, X_OK))
//             {
//                 exe_path = make_exepath((info->path)[index], (info->cmd)[info->argc - i - 1][0]);
//                 index++;
//             }
//             execve(exe_path, (info->cmd)[info->argc - i - 1], info->envp);
//         }
//     }
// }

// void    make_info_cmd(t_info *info, char **argv, int argc)
// {
//     int i;
//     int j;

//     i = 1;
//     j = 0;
//     // if (info->heredoc_flag)
//     // {
//     //     argc = argc - 1;//inputfile用のmalloc
//     //     info->argc = info->argc - 1;//dopipesでinfo->cmdの一番左のコマンドを最後に実行できるように
//     //     i = 2;//本当はi = 3がいいんだけど、here_docの時はiも増えるから
//     // }
//     info->cmd = (char ***)malloc(sizeof(char **) * argc);
//     while (argv[i])
//     {
//         // if (info->heredoc_flag && j == 0)
//         // {
//         //     info->cmd[j] = (char **)malloc(sizeof(char *) * 2);
//         //     info->cmd[j][0] = ft_strdup("input.txt");
//         //     info->cmd[j][1] = NULL;
//         // }
//         // else
//             info->cmd[j] = ft_split(argv[i], ' ');
//         i++;
//         j++;
//     }
//     info->cmd[j] = NULL;

//     // i = 0;
//     // while (info->cmd[i])
//     // {
//     //     j = 0;
//     //     while(info->cmd[i][j])
//     //     {
//     //         printf("%s\n", info->cmd[i][j]);
//     //         j++;
//     //     }
//     //     i++;
//     // }
// }

// void    run_heredoc(t_info *info, char **argv, int argc)
// {
//     char    *buf;
    
//     // (info->argc)--;
//     info->output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT | O_APPEND), 0777);
//     make_info_cmd(info, argv, argc);
//     info->input_fd = open((info->cmd)[0][0], (O_WRONLY | O_CREAT), 0777);
//     while (1)
//     {
//         write(STDOUT, "> ", 2);
//         buf = get_next_line(STDIN);
//         if (!ft_strncmp(argv[2], buf, ft_strlen(argv[2])))
//             break ;
//         write(info->input_fd, buf, ft_strlen(buf));
//         free(buf);
//     }
//     free(buf);
//     close(info->input_fd);
//     info->input_fd = open((info->cmd)[0][0], O_RDONLY);
//     multiple_pipes(info);
// }

// int pipex(int argc, char *argv[])
// {
//     t_info  info;
//     extern char	**environ;
//     int     i;
//     int			wstatus;
    
//     info.argc = argc - 1;
//     info.envp = environ;
//     if (!ft_strncmp(argv[1], "here_doc", 8))
//     {
//         info.heredoc_flag = 1;
//         run_heredoc(&info, argv, argc);
//     }
//     else
//     {
//         info.heredoc_flag = 0;
//         info.input_fd = open(argv[1], O_RDONLY);
//         info.output_fd = open(argv[argc - 1], (O_WRONLY | O_CREAT | O_TRUNC), 0777);
//         make_info_cmd(&info, argv, argc);
//         multiple_pipes(&info);
//     }
//     safty_free(info.path);
//     if  (info.heredoc_flag)
//         unlink(info.cmd[0][0]);
//     i = 0;
//     while (info.cmd[i])
//     {
//         safty_free(info.cmd[i]);
//         i++;
//     }
//     free(info.cmd);
//     close(info.input_fd);
//     close(info.output_fd);
//     return (0);
// }
char    *make_exepath(char *path, char *command)
{
    char    *exe_path;
    char    *tmp;

    tmp = ft_strjoin(path, "/");
    exe_path = ft_strjoin(tmp, command);
    free(tmp);
    return (exe_path);
}

int count_splitable(t_info *info, int start, int end)
{
    int count;

    count = 0;
    while (start < end)
    {
        if (ft_strchr(info->cmd[start], ' '))
            count++;
        start++;
    }
    return (count);
}

// char    *ft_strndup(char *str, int n)
// {
//     char    *ret;

//     ret = (char *)malloc(sizeof(char) * (n + 1));
//     if (!ret)
//         return (NULL);
//     while (n >= 0)
//     {
//         *ret = *str;
//         ret++;
//         str++;
//         n--;
//     }
//     *ret = '\0';
//     return (ret);
// }

void    dopipes(int i, t_info *info)
{
    pid_t   ret;
    int     pipefd[2];
    char    *exe_path;
    int     argv_index;
    int     pipe_index;
    char    **tmp;
    
    exe_path = NULL;
    argv_index = 0;
    if (i == 0)//一番右のコマンド
    {
        pipe_index = info->pipe_place[0] + 1;
        info->argv = (char **)malloc(sizeof(char *) * (info->argc - info->pipe_place[0] + count_splitable(info, pipe_index, info->argc)));
        if (info->argv == NULL)
            fatal_error("malloc");
        while (pipe_index < info->argc)
        {
            if (ft_strchr(info->cmd[pipe_index], ' '))
            {
                tmp = ft_split(info->cmd[pipe_index], ' ');
                while (*tmp)
                {
                    info->argv[argv_index] = ft_strdup(*tmp);
                    argv_index++;
                    tmp++;
                }
            }
            else
                info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
            argv_index++;
            pipe_index++;
        }
        info->argv[argv_index] = NULL;
        // while (*(info->argv))
        // {
        //     printf("%s\n", *info->argv);
        //     (info->argv)++;
        // }
        // exit(1);
    }
    else if (info->pipe_count == i)//一番左のコマンド
    {
        safty_free(info->argv);
        pipe_index = 0;
        info->argv = (char **)malloc(sizeof(char *) * (info->pipe_place[i - 1] + 1));
        if (info->argv == NULL)
            fatal_error("malloc");
        while (pipe_index < info->pipe_place[i - 1])
        {
            info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
            argv_index++;
            pipe_index++;
        }
        info->argv[argv_index] = NULL;
        while (*(info->argv))
        {
            printf("%s\n", *info->argv);
            (info->argv)++;
        }
        exit(1);
    }
    else//それ以外の場所
    {
        safty_free(info->argv);
        pipe_index = info->pipe_place[i] + 1;
        info->argv = (char **)malloc(sizeof(char *) * (info->pipe_place[i - 1] - info->pipe_place[i]));
        if (info->argv == NULL)
            fatal_error("malloc");
        while (pipe_index < info->pipe_place[i - 1])
        {
            info->argv[argv_index] = ft_strdup(info->cmd[pipe_index]);
            argv_index++;
            pipe_index++;
        }
        info->argv[argv_index] = NULL;
        // while (*(info->argv))
        // {
        //     printf("%s\n", *info->argv);
        //     (info->argv)++;
        // }
        // exit(1);
    }
}

char    **make_path_list(char **envp)
{
    char    *pointer_path;

    while (ft_strncmp(*envp, "PATH", 4))
        envp++;
    pointer_path = ft_strchr(*envp, '=');
    return (ft_split(pointer_path + 1, ':'));
}

void    multiple_pipes(t_info *info)
{
    pid_t   ret;

    // printf("%s\n", info->argv[0]);

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
    // while (*info->argv)
    //     {
    //         printf("%s\n", *info->argv);
    //         info->argv++;
    //     }
    // printf("%s\n", info->argv[0]);
    // printf("%s\n", info->argv[1]);
    // exit(1);
}

int count_pipe(t_info *info)
{
    int count;
    int i;

    i = 0;
    count = 0;
    // printf("%s, %d\n", __FILE__, __LINE__);
    while (info->cmd[i])
    {
        // printf("%s, %d\n", __FILE__, __LINE__);
        // printf("%c\n", info->cmd[i][0]);
        if (info->cmd[i][0] == '|' && ft_strlen(info->cmd[i]) == 1)
            count++;
        i++;
    }
    // printf("%s, %d\n", __FILE__, __LINE__);
    return (count);
}

int *place_pipe(t_info *info)
{
    int    *ret;
    int i;
    int j;

    ret = (int *)malloc(sizeof(int) * (info->pipe_count));
    if (ret == NULL)
        fatal_error("malloc");
    i = info->argc - 1;
    j = 0;
    // ret[j++] = 0;
    // ret[info->pipe_count + 1] = info->argc - 1;
    while (i >= 0)
    {
        if (*(info->cmd[i]) == '|' && ft_strlen(info->cmd[i]) == 1)
        // {
            ret[j++] = i;
            // printf("%d, %d\n", i, j);  
        // }
        i--;
    }
    // exit(1);
    return (ret);
}

void    info_init(t_info *info, int argc, char **argv)
{
    extern char **environ;

    info->input_fd = 0;
    info->output_fd = 0;
    // cmdcount(&argc, argv);
    info->argc = argc;//cmdの数
    // printf("|%d|\n", argc);
    info->cmd = argv;
    info->pipe_count = count_pipe(info);
    info->path = make_path_list(environ);
    info->envp = environ;
    info->pipe_place = place_pipe(info);
    // int i = 0;
    // while (i < info->pipe_count)
    // {
    //     printf("%d\n", info->pipe_place[i]);
    //     i++;
    // }
    // exit(1);
}

int pipex(int argc, char **argv)
{
    t_info  info;
    int     i;
    int			wstatus;
    
    info_init(&info, argc, argv);
    // printf("%d\n", info.pipe_count);
    // exit(1);
    // printf("%s, %d\n", __FILE__, __LINE__);
    multiple_pipes(&info);
    // printf("%s, %d\n", __FILE__, __LINE__);
    // i = 0;
    // while (info.cmd[i])
    // {
    //     printf("%s\n", info.cmd[i]);
    //     i++;
    // }
    // printf("%d\n", info.argc);
    // exit(1);
    return (0);
}