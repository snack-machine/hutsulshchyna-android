import QtQuick 2.0

/**
  * adapted from StackOverflow:
  * http://stackoverflow.com/questions/26879266/make-toast-in-android-by-qml
  * @brief Manager that creates Toasts dynamically
  */
ListView {
    /**
      * Public
      */

    /**
      * @brief Shows a Toast
      *
      * @param {string} text Text to show
      * @param {real} duration Duration to show in milliseconds, defaults to 3000
      */
//    function show(text, duration) {
//        model.insert(0, {text: text, duration: duration});
//    }
    function show(text, duration, colorR) {
        model.insert(0, {text: text, duration: duration, colorRect: colorR});
    }

    /**
      * Private
      */

    id: root

    z: Infinity
    spacing: 5
    anchors.fill: parent
    anchors.bottomMargin: 10
    verticalLayoutDirection: ListView.BottomToTop

    interactive: false

    displaced: Transition {
        NumberAnimation {
            properties: "y"
            easing.type: Easing.InOutQuad
        }
    }

    delegate: Toast {
        Component.onCompleted: {
//            show(text);
            show(text, duration, colorRect);
//            if (typeof duration === "undefined") {
//                show(text);
//            }
//            else {
//                show(text, duration);
//            }
        }
    }

    model: ListModel {id: model}
}
