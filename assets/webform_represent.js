/**
 * @file
 *   JavaScript behaviors for the front-end display of Represent Webform
 *   components.
 *
 * @see http://drupal.org/node/304258
 */

(function ($) {

Drupal.behaviors.webformRepresent = {
  attach: function (context, settings) {
    var postal_code_form_key,
        blockError = function ($target, message) {
          $target.before('<div class="webform-represent-alert webform-represent-alert-error">' + message + '</div>');
        },
        inlineError = function ($target, message) {
          $target.after('<span class="webform-represent-alert webform-represent-alert-error">' + message + '</span>');
        },
        sets = [];

    // Assume that representative sets are unique across Represent components.
    $.each(Drupal.settings.webform_represent.lookups, function (form_key, data) {
      sets.push(data.options_source);
    });

    $.each(Drupal.settings.webform_represent.lookups, function (form_key, data) {
      var key = data.postal_code_component.replace('_', '-');

      // Assume that all Represent components have the same postal code component.
      if (postal_code_form_key === undefined) {
        postal_code_form_key = key;
      }
      else if (postal_code_form_key !== key) {
        window.console && console.log('Multiple postal code components (' + postal_code_form_key + ' and ' + key + ')');
      }

      // Hide the Represent components by default.
      $('#webform-component-' + form_key.replace('_', '-'), context).hide();
    });

    var $postal_code_component = $('#webform-component-' + postal_code_form_key, context),
        $postal_code_input = $('#edit-submitted-' + postal_code_form_key, context);

    var change = function () {
      var postal_code = $postal_code_input.val();

      // Remove any existing alerts.
      $postal_code_component.find('.webform-represent-alert').remove();

      if (postal_code) {
        if (postal_code.toUpperCase().replace(/[^A-Z0-9]/g, '').match(/^[ABCEGHJKLMNPRSTVXY][0-9][ABCEGHJKLMNPRSTVWXYZ][0-9][ABCEGHJKLMNPRSTVWXYZ][0-9]$/)) {
          $postal_code_input.after('<span class="webform-represent-alert"><img src="' + Drupal.settings.basePath + Drupal.settings.webform_represent.spinner + '" alt=""> ' + Drupal.t('Searching representatives') + '</span>')

          $.ajax({
            url: Drupal.settings.basePath + 'webform_represent/ajax',
            data: {
              'postal_code': postal_code,
              'sets[]': sets
            },
            complete: function () {
              $postal_code_component.find('.webform-represent-alert').remove();
            },
            error: function () {
              $.each(Drupal.settings.webform_represent.lookups, function (form_key, data) {
                $('#webform-component-' + form_key.replace('_', '-'), context).show();
                blockError($('#edit-submitted-' + form_key.replace('_', '-')), Drupal.t('We were unable to find your representative by postal code. Please select from the list below.'));
              });
            },
            success: function (response) {
              $.each(Drupal.settings.webform_represent.lookups, function (form_key, data) {
                var $component = $('#webform-component-' + form_key.replace('_', '-')),
                    $input = $('#edit-submitted-' + form_key.replace('_', '-'));

                $component.find('.webform-represent-alert').remove();

                $('#webform-component-' + form_key.replace('_', '-'), context).show();

                var emails = response[data.options_source];
                // For now, if multiple representatives match, use the first.
                if (emails) {
                  $input.val(emails[0]);
                }
                else {
                  blockError($input, Drupal.t('We were unable to find your representative by postal code. Please select from the list below.'));
                }
              });
            },
            dataType: 'json'
          });
        }
        else {
          inlineError($postal_code_input, Drupal.t('@postal_code is not a valid postal code', {
            '@postal_code': postal_code
          }));
        }
      }
    }

    // If page loads with a postal code.
    if ($postal_code_input.val()) {
      change();
    }

    // If we assume only one postal code component, this may add multiple handlers.
    $postal_code_input.change(change);
  }
};

})(jQuery);
