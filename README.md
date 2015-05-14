== What? ==

1. Get an Android or iOS smartphone or tablet.
2. Get an UP band.
3. Move.
4. Sync your UP with your device.
5. ?
6. Profit.


== Installation ==

* Download the module and extract it to into sites/all/modules in your Drupal
  installation.
* Install composer, if you don't yet have it installed.
  See: https://getcomposer.org/download/
* Run `composer install --no-dev' to download the Jawbone API library.
* Enable the UP module via admin/modules on your Drupal site.
* Go to admin/config/services/up on your Drupal site.

== Configuration ==

* Login at https://jawbone.com/up/developer/
* Create an Organiation.
* Create an Application.
  - Enter a name and description for your app.
  - Enter your website URL.
  - Enter the Redirect URL from the module settings page in the Authorization
    URL field. This does *not* needs to be https.
  - Enter the your website URL in the OAuth Redirect URIs field.
  - Save your application.
* Enter the client id and app secret in the appropriate fields on your Drupal
  and save the settings.
* Edit your user profile and add your UP band

== Usage ==

Once you install and enable the views module, you'll be able to create views of
your UP bands and activity summaries.

=== NOTE ===

Do not be tempted to publish your application! If you publish it you won't be
able to edit it anymore, and you won't be able to use a non-HTTPS redirect url.

The module will work fine, even if you don't publish the application.
