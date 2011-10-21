
Drush build
-----------
Drush build is basically a wrapper script for several other drush libraries.
It's main purpose is automation and conveniency. To make the automation as easy
as possible one should add command-specific settings into ones drushrc.php file
to get some default values that keep repeating from site to site.

Drush build commands
--------------------

    drush build
      
        Builds a Drupal site with drush make and site-install and also has the
        option to generate a local alias file for the build. This command will
        prompt the user to input all settings required and not supplied from
        the argument list or the configuration files.

    drush deploy

        Deploy a Drupal website to a remote host, generate a remote alias for
        it and download drush on the remote server. There's also options to
        push a public key to the remote host for easier access in the future.
        This command will prompt the user to input all settings required and
        not supplied from the argument list or the configuration files.

        Planned features is bitbucket repository creation.

    drush generate-alias mysite local
      
        Will generate an alias file based on the argument list passed in as
        well as the configuration files. This command will NOT prompt the user.

    drush create-repository reponame

        Create a bitbucket git repository. Has options to also initialize a
        local git repository and and bitbucket as a remote named origin.
        
        The intention of this command has not really been defined yet so the
        API will change in the future.

