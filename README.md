# UGR_Summer_2018
Repository for Summer 2018 Undergraduate Research Code


## Git Quick Start <-- If you are new to GitHub, read this first!

I did a little write up on the git commands I usually use to maintain a repository. Git has other features like branches which are useful for larger projects, but since we only have a few people working on this right now, these should be enough. Depending on the settings you used when you installed git, it should come with git bash. Git Bash will allow you to run some unix commands on windows. Git Desktop has most of the same features as the bash prompt, but it does not give you access to all of the advanced commands that git bash does.  Also, the commands seemed more straightforward to me to use than the desktop so I've just stuck with that. 

Before running these commands, you'll want to open git bash and navigate to the directory on your local machine where you want the repository stored. git status is a good way to check that you are in the right directory before you start working. Git also synchronizes with Pycharm's gui, which is a really nice way to use it if you're working in Python. 

### Setup - these steps only need to be done once
a. ) [git install link](https://git-scm.com/downloads)- First, make sure you have git installed. Github desktop is different from git, and actually uses git to sync with the repository. On Windows, when the install prompts you, make sure you have git bash selected to give you access to unix commands. 
 
b. ) `[git clone](https://git-scm.com/docs/git-clone) https://github.com/HRG-Lab/UGR_Summer_2018.git` - This creates a local copy of the repository which is identical to the one stored on github


### Maintaining the repository - you'll use these commands more regularly. 
Once you've cloned the repository using *git bash,* you can *cd* (Change directory) into the folder on your computer containing the local copy of this repository. <br>
Note: <filenames> can also refer to relative file-paths from the repository root directory. 
  
a.) `[git pull](https://git-scm.com/docs/git-pull)` - pulls the latest changes from the repository backup on Github to your local machine. The easiest way to make sure you aren't erasing someone else's changes is to run git pull each time you start working, and immediately before committing new changes. 

b.) `[git status](https://git-scm.com/docs/git-status)` - checks the status of your local repository against the repository backup on github. This should tell you whether you are behind or ahead of the GitHub copy repository. I usually run this between each command I run here. 

c.) `[git add](https://git-scm.com/docs/git-add) <filename(s)>` - adds a new file to git's repository tracker. You only need to run this each time you create a new file

d.) [git commit](https://git-scm.com/docs/git-commit) <filename(s)> - commit stages changed and newly added files to be uploaded to the repository. Commit will check for any synchronization errors or changes that have been newly added since your last pull

e.) `[git push](https://git-scm.com/docs/git-push)` - push uploads all committed changes to the repository. 

f.) `[git merge](https://git-scm.com/docs/git-merge)` - When someone else's changes have been pushed to the repository since the last time you've pulled, git will force you to do a merge command to reconcile the differences before pushing your changes.  
