#!/usr/bin/env python3

import os
import re


#
# This file sets a package name for the some android app
#


def apply_replace(src_file, dst_file, template: dict):
    """reads in the src file, applies the replace list and saves the generated file under dst (and creates all dirs)"""
    file = open(src_file, 'r')
    text = file.read()
    file.close()

    for item in template:
        text = re.sub(item, template[item], text)

    os.makedirs(os.path.dirname(dst_file), exist_ok=True)
    file = open(dst_file, 'w')
    file.write(text)
    file.close()


def apply_replace_on_file(file, template: dict):
    apply_replace(file, file, template)


#
# Template
#

# the templates needs a website like structure like de.horsimann.tea for horsimann.de/tea


DOMAIN_NAMESPACE = 'de'
DOMAIN = 'horsimann'
APP = 'Tea'
APP_LOWER = 'tea'
LOGO = 'debug'

USE_FULLSCREEN = False
USE_ADMOB = False
USE_BILLING = False



def run_template():
    print('applying template')
    import shutil

    if os.path.exists('out'):
        shutil.rmtree('out')

    print('copying project...')
    shutil.copytree('in', 'out/APP')

    print('copying include...')
    shutil.copytree('../include', 'out/APP/app/jni/mia/include')
    print('copying src...')
    shutil.copytree('../src', 'out/APP/app/jni/mia/src')
    print('copying res...')
    shutil.copytree('../res', 'out/APP/app/src/main/assets/res')
    
    print('copying logo...')
    logo_dirs = ['mdpi', 'hdpi', 'xhdpi', 'xxhdpi', 'xxxhdpi']
    for ld in logo_dirs:
        src = '../logo/' + LOGO + '/mipmap-' + ld
        dst =  'out/APP/app/src/main/res/mipmap-' + ld
        shutil.copytree(src, dst)
    

    print('replacing package names...')
    TEMPLATE = {
        '@@@package_underscored@@@': DOMAIN_NAMESPACE + '_' + DOMAIN + '_' + APP_LOWER,
        '@@@package_dotted@@@': DOMAIN_NAMESPACE + '.' + DOMAIN + '.' + APP_LOWER,
        '@@@package_slashed@@@':  DOMAIN_NAMESPACE + '/' + DOMAIN + '/' + APP_LOWER,
        '@@@app_name@@@': APP
    }
    
    if USE_FULLSCREEN:
        TEMPLATE['//@@@USE_FULLSCREEN@@@'] = ''
        TEMPLATE['#@@@USE_FULLSCREEN@@@'] = ''
        TEMPLATE['<!--@@@USE_FULLSCREEN@@@'] = ''
        TEMPLATE['@@@USE_FULLSCREEN@@@-->'] = ''
    else:
        TEMPLATE['//@@@USE_WINDOW@@@'] = ''
        TEMPLATE['#@@@USE_WINDOW@@@'] = ''
        TEMPLATE['<!--@@@USE_WINDOW@@@'] = ''
        TEMPLATE['@@@USE_WINDOW@@@-->'] = ''
        
    if USE_ADMOB:
        TEMPLATE['//@@@USE_ADMOB@@@'] = ''
        TEMPLATE['#@@@USE_ADMOB@@@'] = ''
        TEMPLATE['<!--@@@USE_ADMOB@@@'] = ''
        TEMPLATE['@@@USE_ADMOB@@@-->'] = ''
        
    if USE_BILLING:
        TEMPLATE['//@@@USE_BILLING@@@'] = ''
        TEMPLATE['#@@@USE_BILLING@@@'] = ''
        TEMPLATE['<!--@@@USE_BILLING@@@'] = ''
        TEMPLATE['@@@USE_BILLING@@@-->'] = ''

    apply_replace_on_file('out/APP/app/jni/mia/Android.mk', TEMPLATE)
    apply_replace_on_file('out/APP/app/src/main/java/de/horsimann/mia/Main.java', TEMPLATE)
    apply_replace_on_file('out/APP/app/src/main/AndroidManifest.xml', TEMPLATE)
    apply_replace_on_file('out/APP/app/build.gradle', TEMPLATE)
    apply_replace_on_file('out/APP/app/src/main/res/values/strings.xml', TEMPLATE)
    apply_replace_on_file('out/APP/app/src/main/res/values/styles.xml', TEMPLATE)

    print('renaming package dirs...')
    shutil.move('out/APP/app/src/main/java/de/horsimann/mia', 'out/APP/app/src/main/java/de/horsimann/' + APP_LOWER)
    shutil.move('out/APP/app/src/main/java/de/horsimann/', 'out/APP/app/src/main/java/de/' + DOMAIN)
    shutil.move('out/APP/app/src/main/java/de/', 'out/APP/app/src/main/java/' + DOMAIN_NAMESPACE)
    shutil.move('out/APP', 'out/'+APP_LOWER)

    print('finish')

    print('')
    print('~ '*32)
    print('')
    print('Start the some Hello World App with Android Studio and test it')
    print('')
    
    if USE_ADMOB:
        print('AdMob: Replace your ad app id in -> out/'+APP_LOWER+'/app/src/main/AndroidManifest.xml')
        print('AdMob: Replace your ad reward id in -> out/'+APP_LOWER+'/app/src/main/java/'+DOMAIN_NAMESPACE+'/'+DOMAIN+'/'+APP_LOWER+'/SDLActivity.java')
        print('')
    if USE_BILLING:
        print('Billing: Replace your billing product ids in -> out/'+APP_LOWER+'/app/src/main/java/'+DOMAIN_NAMESPACE+'/'+DOMAIN+'/'+APP_LOWER+'/SDLActivity.java')
        print('')
    
    print('You should now be ready to compile your own App in AndroidStudio')

    # checks for anything that's not alphanumeric, underscores, or slashes (file separators)
    if re.search(r'[^\w\-/]', os.getcwd()):
        print('')
        print('WARNING, GENERATED DIRECTORY CONTAINS A SPECIAL CHARACTER OR SPACES!')
        print('  MOVE IT TO ANOTHER LOCATION!')
        print('    pwd is: <' + os.getcwd() + '>')
        print('    windows C:\\android\\\\mia or smth)')

    # wait for user input
    input('')


if __name__ == '__main__':
    import sys
    
    def print_help():
        print('Usage: python', sys.argv[0], '<DOMAIN_NAMESPACE> <DOMAIN> <APP> [logo=debug] [--fullscreen] [--admob] [--billing]')
        print(' e.g.: python', sys.argv[0], 'de horsimann Tea')
        exit(1)
    
    args = list(sys.argv[1:])

    if len(args) < 3 or len(args) > 6:
        print_help()

    DOMAIN_NAMESPACE = args[0]
    DOMAIN = args[1]
    APP = args[2]
    APP_LOWER = APP.lower()
    
    if '_' in DOMAIN_NAMESPACE or '_' in DOMAIN or '_' in APP:
        print("package name invalid ( '_' not valid )")
        print_help()
    
    args = args[3:]
    
    if len(args) > 0 and not args[0].startswith('--'):
        LOGO = args[0]
        args = args[1:]
    
    if '--fullscreen' in args:
        USE_FULLSCREEN = True
    
    if '--admob' in args:
        USE_ADMOB = True
        
    if '--billing' in args:
        USE_BILLING = True

    print('DOMAIN_NAMESPACE =', DOMAIN_NAMESPACE)
    print('DOMAIN =', DOMAIN)
    print('APP =', APP)
    print('APP_LOWER =', APP_LOWER)
    print('LOGO =', LOGO)
    print('--fullscreen =', USE_FULLSCREEN)
    print('--admob =', USE_ADMOB)
    print('--billing =', USE_BILLING)

    run_template()