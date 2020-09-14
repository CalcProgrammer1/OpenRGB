# Contribution Guide
Help is always welcome. If you want to contribute to hueplusplus, please read these guidelines

## Request feature / Report bug
To request a feature or report a bug, create an issue using the templates.

## Making changes
If you want to add a new feature or fix a bug, first check out the development branch and open and closed issues.
Maybe the feature already exists and you would just do duplicate work.

Also use the development branch as the base for your feature branch, because all pull requests are first rebased into development.

## Pull requests
When creating a pull request, be sure to choose development as the target.
You might need to rebase on development again and merge in new changes.

### Keeping up with changes
While you are working on your pull request or your forked branch it might occur that 
someone has force pushed the development branch, from which you started your feature branch. 
In that case you will need to rebase onto the force pushed branch. For that you need to follow these steps:

1.  Switch to the development branch
```
git checkout development
```
2.  Add this repository as your remote upstream
```
git remote add upstream git@github.com:enwi/hueplusplus.git
```
3.  Fetch all changes
```
git fetch upstream
```
4.  Reset your development branch and replace it with our (force pushed) version
```
git reset --hard upstream/development
```
> If you have for some reason made changes to your development branch do a rebase pull to preserve them
> ```
> git pull --rebase upstream/development
> ```
5.  Switch back to your feature branch
```
git checkout name-of-your-feature-branch
```
6.  Rebase your changes on to the new development branch
```
git rebase development
```
7.  Force push your changes (because you are diverged now)
```
git push --force
```


## Code style
The code is formatted using clang-format. If you do not want to use it yourself, try to keep your style consistent with the other code
so not too many reformats are necessary.
